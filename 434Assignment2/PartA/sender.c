/*
** talker.c -- a datagram "client" demo
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
#include <pthread.h>

pthread_t RLThread;
int sockfd;
struct sockaddr_storage their_addr;
char *Messages[256];
int messageAck[256];

void* responseListen(){
	int MAXBUFLEN = 512;
	char buf[MAXBUFLEN];
	int numbytes;
	int responseToken;
	socklen_t addr_len;

	addr_len = sizeof their_addr;
	while(1){
		if((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1){
			perror("recvfrom");
			exit(1);
		}
		char *token = strtok(buf, "*");
		responseToken = atoi(token);
		token = strtok(NULL, "*");
		char *responseMessage = token;
		if(strcmp(responseMessage, "SUCCESS")){
			printf("Received Successful response for message #: %i", responseToken);
			messageAck[responseToken] = 1;
		}


	}
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    int i;
    char *SERVERPORT;
    int MAXWINDOWSIZE;
    int TIMEOUTVALUE;
    int bytes_read;
    size_t nbytes = 512;
    int currentWindowVal = 0;
    char *input;
    char *toSend;

    for(i = 0; i < 256; i++){
    	messageAck[i] = 0;
    }

    /*IP Address, Port Number, Max Sending Size, Timeout Value*/
    if (argc != 5) {
        fprintf(stderr,"usage: receiver IP address, port number, window size, timeout value (seconds)\n");
        exit(1);
    }

    SERVERPORT = argv[2];
    MAXWINDOWSIZE = atoi(argv[3]);
    TIMEOUTVALUE = atoi(argv[4]);


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
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
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }
    freeaddrinfo(servinfo);

    pthread_create(&RLThread, NULL, &responseListen, NULL);

    while(1){
		input = (char *)malloc(nbytes + 1);
		bytes_read = getline(&input, &nbytes, stdin);
		if(bytes_read == -1){
			fprintf(stderr, "ERROR READING FROM TERMINAL");
			exit(1);
		}
		input[strcspn(input, "\r\n")] = 0;
		if(strcmp(input, "exit") == 0){
			break;
		}
	   toSend = (char *)malloc(nbytes + 1);
	   sprintf(toSend, "%d", currentWindowVal);
	   strcat(toSend, "*");
	   strcat(toSend, input);

		if ((numbytes = sendto(sockfd, toSend, strlen(toSend), 0,
				 p->ai_addr, p->ai_addrlen)) == -1) {
			perror("talker: sendto");
			exit(1);
		}

		printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
		if(currentWindowVal >= MAXWINDOWSIZE){
			currentWindowVal = 0;
		}else{
			currentWindowVal++;
		}
    }
    close(sockfd);

    return 0;
}
