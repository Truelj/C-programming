/* A friend, who has been waiting on the port, chats with another friend */
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <string.h>
#include <unistd.h> /* for close() */

#define RCVBUFSIZE 5000 /* Size of receive buffer */
#define USERNAME_LENGTH 20
void DieWithError(char *errorMessage); /* Error handling function */

void chatWithFriend(int sock, char *friend_name, char *user){
	char receivedBuffer[RCVBUFSIZE + 1]; /* buffer for received string*/
	char message[RCVBUFSIZE + 1]; /* buffer for sent message */
	char end[RCVBUFSIZE + 1];
	int bytesRcvd;

	strcpy(end, "bye\n");
	/* Continue the conversation if neither the sending message and the received message is "bye" */
	while(strcmp(end, message) != 0 && strcmp(end, receivedBuffer) != 0){
		memset(receivedBuffer, 0, sizeof receivedBuffer);
		printf("Type \"bye\"to stop the conversation\n");

	/* Receive the friend's message */
	/* Receive up to the buffer size bytes from the sender */
    	if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        	DieWithError("recv() failed or connection closed prematurely");
    	printf("%s: %s" ,friend_name, receivedBuffer); /* Print friend's name and message */
    /* Send message  to friend if the received message is not "bye" */
    	if(strcmp(end, receivedBuffer) != 0){
    		printf("%s: ", user); /* Print user's name */
			fgets(message, sizeof(message), stdin); /* Get user's message */
    		if ( send(sock, message, strlen(message), 0) != strlen(message))
        		DieWithError("send() sent a different number of bytes than expected");
    	}

	}	

	printf("disconnected from my friend\n");
	close(sock);
}