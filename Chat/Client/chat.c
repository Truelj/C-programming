/* The client chats to a friend, who is waiting on a port */
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <string.h>

#define RCVBUFSIZE 50 /* Size of receive buffer */
#define USERNAME_LENGTH 20
void DieWithError(char *errorMessage); /* Error handling function */

void chat(int sock, char *friend_name, char *user){
	char receivedBuffer[RCVBUFSIZE + 1]; /* buffer for received string*/
	char message[RCVBUFSIZE + 1]; /* buffer for sent message */
	char end[RCVBUFSIZE + 1];
	int bytesRcvd;
	
    /* Enter "bye" to end the conversation */
	strcpy(end, "bye\n");
	/* Continue the conversation if neither the sending message and the received message is "bye" */
	while(strcmp(end, message) != 0 && strcmp(end, receivedBuffer) != 0){
		printf("Type \"bye\"to stop the conversation\n");
		printf("%s: ", user);
	/* Send message to friend */
		fgets(message, sizeof(message), stdin);
    	if (send(sock, message, strlen(message), 0) != strlen(message))
        	DieWithError("send() sent a different number of bytes than expected");

        memset(receivedBuffer, 0, sizeof receivedBuffer);
	/* Receive the friend's message if the sending message is not "bye" */
	/* Receive up to the buffer size bytes from the sender */
    	if ((strcmp(end, message) != 0) && (bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        	DieWithError("recv() failed or connection closed prematurely");
        printf("%s: " , friend_name); /* Print the friend's name */
        if (strcmp(end, message) != 0){
    		printf("%s" ,receivedBuffer); /* Print the friend's message, which ends with '\n' */
        }else{
        	printf("\n"); /* End the conversation */
        }

	}	
	printf("disconnected from my friend\n");

}