/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 1024 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	char *url = argv[1];
    char protocol[8];
    char hostname[256];
    char path[1024];
    int port = 0; // default port
	char port_char[5];

	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

    // generate output name.
    // char outputFilename[256];
    // generateOutputFilename(url, outputFilename, sizeof(outputFilename));
    // char outputFilename[256] = "output";

    if (sscanf(url, "%7[^:]://%255[^/]/%1023s", protocol, hostname, path) != 3) {
        fprintf(stderr, "Invalid URL format\n");
        return 1;
    }

    if (strcmp(protocol, "http") == 0) {
        port = 80;
    } else if (strcmp(protocol, "https") == 0) {
        port = 443;
    }

	// detect whether there is default port in the request.
	char *portPos = strchr(hostname, ':');
    if (portPos != NULL) {
        port = atoi(portPos + 1);
        *portPos = '\0'; 
    }
	
	sprintf(port_char, "%d", port);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(hostname, port_char, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}


	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	printf("Protocol: %s\n", protocol);
    printf("Hostname: %s\n", hostname);
    printf("Path: %s\n", path);
    printf("Port: %d\n", port);

	freeaddrinfo(servinfo); // all done with this structure
	char request[4096];
	snprintf(request, sizeof(request), "GET /%s HTTP/1.1\r\nUser-Agent: Wget/1.12 (linux-gnu)\r\nHost: %s:%d\r\nConnection: Keep-Alive\r\n\r\n", path, hostname, port);
	printf("Request Header:\r\n%s\r\n", request);

	// send request
	send(sockfd, request, strlen(request), 0);							

	// if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	//     perror("recv");
	//     exit(1);
	// }

	// if (numbytes > 0) {
	//     // 检查响应是否以 "HTTP/1.1 200 OK" 开头
	//     if (strncmp(buf, "HTTP/1.1 200 OK", 15) == 0) {
	//         printf("Received a 200 OK response.\n");
	//     } else {
	//         printf("Received a non-200 response:\n%s\n", buf);
	//         return 1;
	//     }
	// } else {
	//     perror("recv");
	//     exit(1);
	// }

	FILE *output_file = fopen("output", "wb");
	if (output_file == NULL) {
	    perror("Failed to open output file");
	    exit(1);
	}				

	int flag = 1;
	int time = 0;

	while (1) {
	    memset(buf, '\0', MAXDATASIZE);
	    numbytes = recv(sockfd, buf, MAXDATASIZE, 0);
		if (time == 0) {
			time = 1;
			if (numbytes > 0) {
			    // 检查响应是否以 "HTTP/1.1 200 OK" 开头
			    if (strncmp(buf, "HTTP/1.1 200 OK", 15) == 0) {
			        printf("Received a 200 OK response.\n");
			    } else {
			        printf("Received a non-200 response:\n%s\n", buf);
			        return 1;
			    }
			} else {
			    perror("recv");
			    exit(1);
			}
		}
	    if (numbytes > 0) {
	        if (flag) {
	            char *start = strstr(buf, "\r\n\r\n") + 4;
	            flag = 0;
	            fwrite(start, 1, strlen(start), output_file);
	        } else {
	            fwrite(buf, 1, numbytes, output_file);
	        }
	
	        // 刷新文件缓冲区，确保数据写入文件
	        fflush(output_file);
	    } else {
	        fclose(output_file);
	        break;
	    }
	}


	// buf[numbytes] = '\0';
	// printf("client: received '%s'\n",buf);

	close(sockfd);

	return 0;
}

