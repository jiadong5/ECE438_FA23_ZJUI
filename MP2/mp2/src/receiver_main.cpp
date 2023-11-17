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

#define DATA_SIZE 2000
#define BUFF_SIZE 600000
#define TOT_COUNT 300

using namespace std;

struct sockaddr_in si_me, si_other;
int s, slen;
int recvbytes;
char buf[sizeof(packet)];
struct sockaddr_in sender_addr;
socklen_t addrlen;

// FIN: 0; DATA: 1; FINACK:2; ACK:3.
enum PacketType { FIN, DATA, FINACK, ACK };

typedef struct{
    int 	data_size;
    int 	seq_num;
    int     ack_num;
    PacketType msg_type; //DATA 0 SYN 1 ACK 2 FIN 3 FINACK 4
    char    data[DATA_SIZE];
}packet;

void send_ack(int ack_idx, PacketType ack_type) {
    // Implement the logic to send ACK
    packet ack;
    ack.ack_num = ack_idx;
    ack.msg_type = ack_type;

    if (sendto(s, &ack, sizeof(packet), 0, (sockaddr*)&si_other, (socklen_t)sizeof (si_other))==-1){
        diep("ACK's sendto()");
    }
}

void diep(char *s) {
    perror(s);
    exit(1);
}

void reliablyReceive(unsigned short int myUDPport, char* destinationFile) {
    
    slen = sizeof (si_other);
    int ack_index = 0;

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
    if (fp == NULL){
        diep("Cannot open the destination file");
    }

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

