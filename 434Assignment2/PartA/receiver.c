/*
** listener.c -- a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT "30001"    // the port users will be connecting to

#define MAXBUFLEN 512

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    int lastReceivedWindow = -1;
    int currentReceivedWindow;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    size_t nbytes = 512;
    char s[INET6_ADDRSTRLEN];
    char *input;
	char *toSend;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);
    while(1){
		printf("listener: waiting to recvfrom...\n");

		addr_len = sizeof their_addr;
		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
			(struct sockaddr *)&their_addr, &addr_len)) == -1) {
			perror("recvfrom");
			exit(1);
		}
		char *token = strtok(buf, "*");
		currentReceivedWindow = atoi(token);
		token = strtok(NULL, "*");
		char *toPrint = token;

		if(currentReceivedWindow == lastReceivedWindow + 1){
			lastReceivedWindow++;
			printf("listener: got packet from %s\n",
					inet_ntop(their_addr.ss_family,
							get_in_addr((struct sockaddr *)&their_addr),
							s, sizeof s));
			printf("listener: packet is %d bytes long\n", numbytes);
			buf[numbytes] = '\0';
			printf("Frame number: %d; Packet contains: \"%s\"\n", currentReceivedWindow, toPrint);
			printf("Please indicate correct transmission (Y/N)\n");
			getline(&input, &nbytes, stdin);
			input[strcspn(input, "\r\n")] = 0;
			printf("Debugging Statement\n");
			if(strcmp(input, "Y") == 0){
//				toSend = (char *)malloc(nbytes + 1);
//				sprintf(toSend, "%i", currentReceivedWindow);
//				strcat(toSend, "*SUCCESS");
				//sendto(sockfd, toSend, strlen(toSend), 0, p->ai_addr, p->ai_addrlen);
				printf("Message was indicated as correct.\n");
			}else{
				printf("MESSAGE WAS INDICATED INCORRECT:\n");
				printf("listener: packet is %d bytes long\n", numbytes);
				buf[numbytes] = '\0';
				printf("Frame number: %d; Packet contains: \"%s\"\n", currentReceivedWindow, toPrint);
			}
		}else{
			printf("MESSAGE WAS RECEIVED OUT OF ORDER:\n");
			printf("listener: packet is %d bytes long\n", numbytes);
			buf[numbytes] = '\0';
			printf("Frame number: %d; Packet contains: \"%s\"\n", currentReceivedWindow, toPrint);
		}
    }

    close(sockfd);

    return 0;
}
