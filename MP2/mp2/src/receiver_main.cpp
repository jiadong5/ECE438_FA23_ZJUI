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
#include <iostream>
#include <queue>
#include <cstdio>
#include <cstring>

#define DATA_SIZE 4000

using namespace std;

void diep(string s) {
    perror(s.c_str());
    exit(1);
}

struct sockaddr_in si_me, si_other;
int s, slen;
int recvbytes;
struct sockaddr_in sender_addr;
socklen_t addrlen;

// FIN: 0; DATA: 1; FINACK:2; ACK:3.
enum pkt_type_t { FIN, DATA, FINACK, ACK };

/* This struct is for the priority queue */

typedef unsigned long long int long_t;
typedef struct{
    int 	data_size;
    long_t 	seq_num;
    long_t     ack_num;
    pkt_type_t msg_type;
    char    data[DATA_SIZE];
}pkt_t;

struct is_greater {
    bool operator()(pkt_t a, pkt_t b) {
        return  a.seq_num > b.seq_num; 
    }
};

priority_queue<pkt_t, vector<pkt_t>, is_greater> pkt_queue;

void send_ack(int ack_idx, pkt_type_t ack_type) {
    // Implement the logic to send ACK
    pkt_t ack;
    ack.ack_num = ack_idx;
    ack.msg_type = ack_type;
    // Unrelated fields
    ack.data_size = 0;
    ack.seq_num = 0;
    memset(&ack.data, 0,DATA_SIZE);

    if (sendto(s, &ack, sizeof(pkt_t), 0, (sockaddr*)&si_other, (socklen_t)sizeof (si_other))==-1)
        diep("send fail");
}

void reliablyReceive(unsigned short int myUDPport, char* destinationFile) {
    slen = sizeof (si_other);
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        diep("socket");
    memset((char *) &si_me, 0, sizeof (si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(myUDPport);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Now binding\n");
    if (bind(s, (struct sockaddr*) &si_me, sizeof (si_me)) == -1)
        diep("bind");

	/* Now receive data and send acknowledgements */
    FILE* fp = fopen(destinationFile, "wb");  
    if (fp == NULL)
        diep("Cannot open the destination file");

    int ack_base = 0;

    while (true){
        pkt_t recv_pkt;
        if (recvfrom(s, &recv_pkt, sizeof(pkt_t), 0, (sockaddr*)&si_other, (socklen_t*)&slen) == -1)
            diep("recvfrom()");

        // Finish receiving when FIN. End Transmission.
        if (recv_pkt.msg_type == FIN){
            cout << "==FIN==" <<endl;
            send_ack(ack_base, FINACK);
            break;
        }

        if (recv_pkt.msg_type == DATA){
            // receive duplicated pkt.
            if (recv_pkt.seq_num < ack_base){
                cout << "receive duplicated pkt." << endl;
                send_ack(ack_base, ACK);
                continue;
            }
            // receive out of order pkt
            if (recv_pkt.seq_num > ack_base){
                pkt_queue.push(recv_pkt);
                send_ack(ack_base, ACK);
                continue;
            }
            // receive in-order pkt.
            // Write data to file.
            fwrite(recv_pkt.data, sizeof(char), recv_pkt.data_size, fp);
            ack_base += recv_pkt.data_size;
            // only in order and in queue will be written to file.
            while (!pkt_queue.empty() && pkt_queue.top().seq_num == ack_base){
                pkt_t pkt = pkt_queue.top();
                fwrite(pkt.data, sizeof(char), pkt.data_size, fp);
                ack_base += pkt.data_size;
                pkt_queue.pop();
            }
            cout << ack_base << endl;
            send_ack(ack_base, ACK);
        }
    }

    fclose(fp);
    close(s);
    printf("%s received.", destinationFile);
    return;
}

/*
 * 
 */
int main(int argc, char** argv) {

    unsigned short int udpPort;

    if (argc != 3) {
        fprintf(stderr, "usage: %s UDP_port filename_to_write\n\n", argv[0]);
        exit(1);
    }

    udpPort = (unsigned short int) atoi(argv[1]);

    reliablyReceive(udpPort, argv[2]);
}

