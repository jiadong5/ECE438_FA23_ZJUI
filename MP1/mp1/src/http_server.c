/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"   // 等待客户端连接的端口
#define BACKLOG 10     // 允许等待连接的最大数量
#define MAXDATASIZE 4096 // 一次最大读取的字节数

void sigchld_handler(int s)
{
	s++;
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

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
	const char *port;
    
    if (argc == 2) {
        port = argv[1]; // 使用命令行参数中提供的端口号
    } else {
        port = PORT; // 使用默认端口号
    }

	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	char buf[MAXDATASIZE];
    // FILE *fp;

    while (1) {  // 主 accept() 循环
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { // 这是子进程
		    close(sockfd); // 子进程不需要监听套接字
		    recv(new_fd, buf, MAXDATASIZE, 0);
		    char new_buf[MAXDATASIZE];
		    memset(new_buf, '\0', MAXDATASIZE);

		    if (strncmp(buf, "GET ", 4) == 0) {
		        char path[MAXDATASIZE];
		        if (sscanf(buf, "GET /%s HTTP", path) == 1) {
		            printf("Requested path: %s\n", path);
		            FILE *fp = fopen(path, "rb");
		            if (fp != NULL) {
		                strcpy(new_buf, "HTTP/1.1 200 OK\r\n\r\n");

		                // 发送HTTP响应头
		                if (send(new_fd, new_buf, strlen(new_buf), 0) == -1) {
		                    perror("send");
		                    exit(1);
		                }

		                memset(buf, '\0', MAXDATASIZE);

		                // 发送文件内容
		                while (1) {
		                    int numbytes = fread(buf, sizeof(char), MAXDATASIZE, fp);
		                    if (numbytes > 0) {
		                        if (send(new_fd, buf, numbytes, 0) == -1) {
		                            perror("send");
		                            exit(1);
		                        }
		                        memset(buf, '\0', MAXDATASIZE);
		                    } else {
		                        break;
		                    }
		                }

		                fclose(fp);
		            } else {
		                // 文件不存在，返回404响应
		                strcpy(new_buf, "HTTP/1.1 404 Not Found\r\n\r\n");
		                if (send(new_fd, new_buf, strlen(new_buf), 0) == -1) {
		                    perror("send");
		                    exit(1);
		                }
		            }
		        } else {
		            // 请求不符合预期格式，返回400响应
		            strcpy(new_buf, "HTTP/1.1 400 Bad Request\r\n\r\n");
		            if (send(new_fd, new_buf, strlen(new_buf), 0) == -1) {
		                perror("send");
		                exit(1);
		            }
		        }
		    } else {
		        // 请求不是GET请求，返回400响应
		        strcpy(new_buf, "HTTP/1.1 400 Bad Request\r\n\r\n");
		        if (send(new_fd, new_buf, strlen(new_buf), 0) == -1) {
		            perror("send");
		            exit(1);
		        }
		    }

		    close(new_fd);
		    exit(0);
		}
        close(new_fd);  // 父进程不需要新连接套接字
    }

	return 0;
}

