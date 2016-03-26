#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <string.h>

#define RCVBUFSIZE 50 /* Size of receive buffer */
#define USERNAME_LENGTH 20
void DieWithError(char *errorMessage); /* Error handling function */

void chat(int sock, char *user){
	char userName[USERNAME_LENGTH];/* user name*/
	char receivedBuffer[RCVBUFSIZE + 1]; /* buffer for received string*/
	char message[RCVBUFSIZE + 1]; /* buffer for sent message */
	char end[RCVBUFSIZE + 1];

	int bytesRcvd;
	/* Send name to friend */
	if (send(sock, user, strlen(user), 0) != strlen(user))
        DieWithError("send() sent a different number of bytes than expected");

	strcpy(end, "bye\n");
	while(strcmp(end, message) != 0 && strcmp(end, receivedBuffer) != 0){
		printf("Type \"bye\"to stop the conversation\n");
		printf("%s: ", user);
	/*send message to friend*/
		fgets(message, sizeof(message), stdin);
    	if (send(sock, message, strlen(message), 0) != strlen(message))
        	DieWithError("send() sent a different number of bytes than expected");
    /* Receive the friend's name*/
    /* Receive up to the buffer size bytes from the sender */
    	if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        	DieWithError("recv() failed or connection closed prematurely");
    	receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
    	printf("%s: " ,receivedBuffer); /* Print the buffer */
		memset(receivedBuffer, 0, sizeof receivedBuffer);
	/* Receive the friend's message */
	/* Receive up to the buffer size bytes from the sender */
    	if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        	DieWithError("recv() failed or connection closed prematurely");
    	printf("%s" ,receivedBuffer); /* Print the buffer */
	}	
	printf("disconnected from my friend\n");

}