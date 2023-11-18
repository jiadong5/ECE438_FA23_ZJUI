/* Author: Jiadong Hong */
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

using namespace std;

struct sockaddr_in si_other;
int s, slen;
FILE *fp;


void diep(string s) {
    perror(s.c_str());
    exit(1);
}

/* Marcos */
#define DATA_SIZE 100

enum PacketType { FIN, DATA, FINACK, ACK };
enum State {SLOW_START, CONGESTION_AVOIDANCE, FAST_RECOVERY};
typedef unsigned long long int long_t;
/* Structs  */
typedef struct{
    int 	data_size;
    long_t 	seq_num;
    long_t     ack_num;
    PacketType msg_type; //DATA 0 SYN 1 ACK 2 FIN 3 FINACK 4
    char    data[DATA_SIZE];
}pkt_t;

/* Function declaration */
void UserDataHandler();
void ACKHandler(pkt_t ack);
void dequeue_on_new_ack(pkt_t ack);
void finish();


void set_slow_start();
void set_fast_recovery();

/* Variables */
string hostname;
unsigned short int hostUDPport;
string filename;
unsigned long long int bytesToTransfer;
unsigned long long int all_bytes_read;

time_t timer;
time_t timeout_interval;
time_t EstimatedRTT; // average RTT
time_t DevRTT;


queue<pkt_t> pkt_queue; // Packets that have received ack
long_t cw_base;
long_t cwnd;
long_t sst;
State state;
int dupack;
long_t last_byte_sent;
long_t last_byte_acked;

void print_pkt(pkt_t pkt)
{
    cout << "type: " << pkt.msg_type << " seq_num: "  << pkt.seq_num <<
    " ack_num: " << pkt.ack_num << " data_size: " << pkt.data_size << endl;
}



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
    last_byte_sent = 0;
    last_byte_acked = 0;

    /* TODO: Three handshake */

    UserDataHandler();
    while(1)
    {
        pkt_t recv_pkt;
        if(recvfrom(s, &recv_pkt, sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t*) &slen) == -1)
            diep("Recv fail");
        // If receive last ack, finish
        if(recv_pkt.ack_num == bytesToTransfer)
        {
            finish();
            break;
        }
        ACKHandler(recv_pkt);
    }

    printf("Closing the socket\n");
    close(s);

    return;

}

/*
 * User Data Handler (Make Package, Check Window Size, Send Data via UDP)
 */
void UserDataHandler(){
    // Enqueue messages

    char buf[DATA_SIZE];
    int cnt = 0;
    queue<pkt_t> send_queue = {}; // Packets to be sent
    cout << "CW_BASE:" << cw_base << endl;
    while(cwnd - pkt_queue.size() * DATA_SIZE >= DATA_SIZE && !feof(fp))
    {
        int bytes_to_read = min((long_t) DATA_SIZE, bytesToTransfer - all_bytes_read);

        int bytes_read = fread(buf, sizeof(char), bytes_to_read, fp);
        if(bytes_read == 0) {
            break;
        }
        all_bytes_read += bytes_read;

        // Enqueue new packets
        pkt_t new_pkt = {bytes_read,cw_base + cnt * DATA_SIZE,0,DATA,0};
        memcpy(new_pkt.data, &buf, bytes_read);
        pkt_queue.push(new_pkt);
        send_queue.push(new_pkt);
        cnt++;
    }

    // Transmit based on CW
    while(!send_queue.empty())
    {
        if (sendto(s, &send_queue.front(), sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t)sizeof(si_other)) == -1)
            diep("Send Fail");
        cout << "SEND:" << send_queue.front().seq_num << endl;
        send_queue.pop();
    }
    

}

/*
 * ACK Handler (Update RTT, Update CWND, Update FSM of Congestion Control, Handle Duplicated ACK)
 */
void ACKHandler(pkt_t ack){
    print_pkt(ack);

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
        cwnd += DATA_SIZE;
        last_byte_acked = ack.ack_num;
        if(cwnd >= sst)
            state = CONGESTION_AVOIDANCE;
        dupack = 0;
        dequeue_on_new_ack(ack);
        UserDataHandler();
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
        cwnd += DATA_SIZE * DATA_SIZE / cwnd;
        last_byte_acked = ack.ack_num;
        dupack = 0;
        dequeue_on_new_ack(ack);
        UserDataHandler();
        return;
    }
    if(state == FAST_RECOVERY) {
        if(ack.ack_num < cw_base)
            return;
        // Receive duplicate ack
        if(ack.ack_num == cw_base)
        {
            cwnd += DATA_SIZE;
            UserDataHandler();
        }
        // Receive new ack
        last_byte_acked = ack.ack_num;
        state = CONGESTION_AVOIDANCE;
        dupack = 0;
        cwnd = sst;
        dequeue_on_new_ack(ack);
        UserDataHandler();

        return;
    }

    return;
}

void set_slow_start(){

}

// Shift from slow start or congestion avoidance to fast recovery
void set_fast_recovery(){
    dupack = 0;
    sst = cwnd / 2;
    cwnd = sst +  3 * DATA_SIZE;
    state = FAST_RECOVERY;
    // Retransmit cw_base paket
    if (sendto(s, &pkt_queue.front(), sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t)sizeof(si_other)) == -1)
        diep("Send Fail");
}

void dequeue_on_new_ack(pkt_t ack){
    cw_base = ack.ack_num;
    while(!pkt_queue.empty() && pkt_queue.front().seq_num != cw_base)
        pkt_queue.pop();

}

/*
 * Timeout Handler (Check Timeout, Resend Package, Update FSM of Congestion Control, Restart the clock)
 */
void TimeoutHandler(){
    return;
}


void finish() {
    pkt_t fin_pkt = {0,0,0,FIN,0};
    if (sendto(s, &fin_pkt, sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t)sizeof(si_other)) == -1)
        diep("Send Fail");

    pkt_t recv_pkt;
    while(1) {
        if(recvfrom(s, &recv_pkt, sizeof(pkt_t),0,(sockaddr*)&si_other, (socklen_t*) &slen) == -1)
            diep("Recv fail");
        if(recv_pkt.msg_type == FINACK)
            return;
    }
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


