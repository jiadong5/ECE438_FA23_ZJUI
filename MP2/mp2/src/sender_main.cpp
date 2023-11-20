#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <queue>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <fcntl.h>

using namespace std;

struct sockaddr_in si_other;
int s, slen;
FILE *fp;
void diep(string s) {
    perror(s.c_str());
    exit(1);
}
/* Marcos */
#define DATA_SIZE 14000

/* structs and typedef */
enum pkt_type_t { FIN, DATA, FINACK, ACK };
enum state_t {SLOW_START, CONGESTION_AVOIDANCE, FAST_RECOVERY};
typedef unsigned long long int long_t;
typedef struct{
    int 	data_size;
    long_t 	seq_num;
    long_t     ack_num;
    pkt_type_t msg_type; 
    char    data[DATA_SIZE];
}pkt_t;
typedef struct{
    time_t send_time;
    bool retransmit; // If this packet is a retransmitted packet
}record_t;

/* Function declaration */
void user_data_handler();
void ack_handler(pkt_t ack);
void timeout_handler();
void finish_hander();
/* Auxiliary functions */
void dequeue_on_new_ack(pkt_t ack);
time_t get_cur_time();
void update_time_interval(long_t ack_num);
void retransmit_base();
void set_fast_recovery();

/* Variables */
string hostname;
unsigned short int hostUDPport;
string filename;
unsigned long long int bytesToTransfer;
unsigned long long int all_bytes_read;
bool read_over; // Flag to indicate that all data in file is read

time_t timeout_interval; // Units in microseconds
time_t estimate_rtt; // average RTT
time_t dev_rtt;
unordered_map<long_t, record_t> time_record; // corresponding ack_num -> send time for every packet

queue<pkt_t> pkt_queue; // Packets that have received ack
long_t cw_base;
float cwnd;
float sst;
state_t state;
int dupack;

void reliablyTransfer(char* hostname, unsigned short int hostUDPport, char* filename, unsigned long long int bytesToTransfer) {
    // Open the file
    fp = fopen(filename, "rb");
    if (fp == NULL)
        diep("Could not open file to send.");
	/* Determine how many bytes to transfer */
    slen = sizeof (si_other);
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        diep("socket");
    memset((char *) &si_other, 0, sizeof (si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(hostUDPport);
    if (inet_aton(hostname, &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

	/* Send data and receive acknowledgements on s*/
    pkt_queue = {};
    cw_base = 0;
    cwnd = DATA_SIZE;
    sst = 64 * DATA_SIZE;
    state = SLOW_START;
    dupack = 0;
    all_bytes_read = 0;
    read_over = false;
    bytesToTransfer += 0;
    timeout_interval = 1000000;
    estimate_rtt = timeout_interval; // average RTT
    dev_rtt = 0;

    fcntl(s, F_SETFL, O_NONBLOCK);

    user_data_handler();
    while(1)
    {
        pkt_t recv_pkt;
        if(recvfrom(s, &recv_pkt, sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t*) &slen) == -1)
        {
            if(errno != EAGAIN || errno != EWOULDBLOCK)
                diep("Recv fail");
            else if(!pkt_queue.empty()) {
                timeout_handler();
            }
            continue;
        }
        // If receive last ack, finish
        if(read_over && recv_pkt.ack_num == all_bytes_read)
        {
            finish_hander();
            break;
        }
        ack_handler(recv_pkt);
    }

    printf("Closing the socket\n");
    close(s);

    return;

}

void user_data_handler(){
    char buf[DATA_SIZE];
    queue<pkt_t> send_queue = {}; // Packets to be sent
    while(cwnd - pkt_queue.size() * DATA_SIZE >= DATA_SIZE && !read_over)
    {
        int bytes_to_read = min((long_t) DATA_SIZE, bytesToTransfer - all_bytes_read);
        int bytes_read = fread(buf, sizeof(char), bytes_to_read, fp);
        if(bytes_read == 0) {
            read_over = true;
            break;
        }

        // Enqueue new packets
        pkt_t new_pkt = {bytes_read, all_bytes_read ,0,DATA,0};
        memcpy(new_pkt.data, &buf, bytes_read);
        pkt_queue.push(new_pkt);
        send_queue.push(new_pkt);
        all_bytes_read += bytes_read;
    }
    
    // Transmit based on CW
    while(!send_queue.empty())
    {
        if (sendto(s, &send_queue.front(), sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t)sizeof(si_other)) == -1)
            diep("Send Fail");
        time_record[send_queue.front().seq_num + send_queue.front().data_size] = {get_cur_time(),false};
        send_queue.pop();
    }

    if(all_bytes_read == bytesToTransfer || feof(fp) != 0){
        read_over = true;
    }

}

void ack_handler(pkt_t ack){
    if(state == SLOW_START) {
        if(ack.ack_num < cw_base)
            return;
        // Receive duplicate ack
        if(ack.ack_num == cw_base)
        {
            dupack++;
            if(dupack == 3)
                set_fast_recovery();
            return;
        }
        // Receive new ack
        update_time_interval(ack.ack_num);
        cwnd += DATA_SIZE;
        if(cwnd >= sst)
            state = CONGESTION_AVOIDANCE;
        dupack = 0;
        dequeue_on_new_ack(ack);
        user_data_handler();
        return;
    }
    if(state == CONGESTION_AVOIDANCE) {
        if(ack.ack_num < cw_base)
            return;
        // Receive duplicate ack
        if(ack.ack_num == cw_base)
        {
            dupack++;
            if(dupack == 3)
                set_fast_recovery();
            return;
        }
        // Receive new ack
        update_time_interval(ack.ack_num);
        cwnd += DATA_SIZE * DATA_SIZE / floor(cwnd);
        dupack = 0;
        dequeue_on_new_ack(ack);
        user_data_handler();
        return;
    }
    if(state == FAST_RECOVERY) {
        if(ack.ack_num < cw_base)
            return;
        // Receive duplicate ack
        if(ack.ack_num == cw_base)
        {
            cwnd += DATA_SIZE;
            user_data_handler();
            return;
        }
        // Receive new ack
        update_time_interval(ack.ack_num);
        state = CONGESTION_AVOIDANCE;
        dupack = 0;
        cwnd = sst;
        dequeue_on_new_ack(ack);
        user_data_handler();
        return;
    }
    return;
}


// Shift from slow start or congestion avoidance to fast recovery
void set_fast_recovery(){
    dupack = 0;
    sst = cwnd / 2;
    cwnd = sst +  3 * DATA_SIZE;
    state = FAST_RECOVERY;
    retransmit_base();
}

void dequeue_on_new_ack(pkt_t ack){
    cw_base = ack.ack_num;
    while(!pkt_queue.empty() && pkt_queue.front().seq_num != cw_base)
        pkt_queue.pop();
}

void timeout_handler(){
    pkt_t base_pkt = pkt_queue.front();
    if(get_cur_time() - time_record[base_pkt.seq_num + base_pkt.data_size].send_time >= timeout_interval) {
        state = SLOW_START;
        sst = cwnd / 2;
        dupack = 0;
        cwnd = DATA_SIZE;
        // Retransmit cw_base paket
        retransmit_base();
    }
    return;
}


void finish_hander() {
    pkt_t fin_pkt = {0,0,0,FIN,0};
    if (sendto(s, &fin_pkt, sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t)sizeof(si_other)) == -1)
        diep("Send Fail");
    time_t fin_send_time = get_cur_time();

    pkt_t recv_pkt;
    while(1) {
        if(recvfrom(s, &recv_pkt, sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t*) &slen) == -1)
        {
            if(errno != EAGAIN || errno != EWOULDBLOCK){
                diep("recv error");
            }else{
                if(get_cur_time() - fin_send_time >= timeout_interval) {
                    if (sendto(s, &fin_pkt, sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t)sizeof(si_other)) == -1)
                        diep("Send Fail");
                    fin_send_time = get_cur_time();
                }
            }
            continue;
        }
        if(recv_pkt.msg_type == FINACK)
            return;
    }
}

void retransmit_base()
{
    if (sendto(s, &pkt_queue.front(), sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t)sizeof(si_other)) == -1)
        diep("Send Fail");
    time_record[pkt_queue.front().seq_num + pkt_queue.front().data_size] = {get_cur_time(), true};
}

time_t get_cur_time()
{
    struct timeval cur_time;
    gettimeofday(&cur_time, nullptr);
    time_t microseconds = cur_time.tv_sec * 1000000 + cur_time.tv_usec;
    return microseconds;
}

// Only called after (first) ack
void update_time_interval(long_t ack_num)
{
    if(time_record[ack_num].retransmit == true) 
    {
        time_record.erase(ack_num);
        return;
    }
    time_t sample_rtt = get_cur_time() - time_record[ack_num].send_time;
    // cout << "state: " << state << " sample_rtt: " << sample_rtt << " sendtime: " << time_record[ack_num].send_time << " timeout interval: " << timeout_interval << " ack_num : " << ack_num << " cw_base: " << cw_base << endl;
    estimate_rtt = (time_t)(0.875 * estimate_rtt + 0.125 * sample_rtt);
    dev_rtt = (time_t)(0.75 * dev_rtt + 0.25 * abs(sample_rtt - estimate_rtt));
    timeout_interval = estimate_rtt + 4 * dev_rtt;

    time_record.erase(ack_num);
}
/*
 * main function.
 */
int main(int argc, char** argv) {

    unsigned short int udpPort;
    unsigned long long int numBytes;

    if (argc != 5) {
        fprintf(stderr, "usage: %s receiver_hostname receiver_port filename_to_xfer bytes_to_xfer\n\n", argv[0]);
        exit(1);
    }
    udpPort = (unsigned short int) atoi(argv[2]);
    numBytes = atoll(argv[4]);


    // Assign global variables
    hostname = argv[1];
    hostUDPport = udpPort;
    filename = argv[3];
    bytesToTransfer = numBytes;

    reliablyTransfer(argv[1], udpPort, argv[3], numBytes);
    
    return (EXIT_SUCCESS);
}


