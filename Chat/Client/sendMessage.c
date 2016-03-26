#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <string.h>


#define OPTIONSIZE 1 /* Size of option buffer*/
#define RCVBUFSIZE 50 /* Size of receive buffer */
#define USERNAME_LENGTH 20 /* size of receiver's user name */
void DieWithError(char *errorMessage); /* Error handling function */

void sendMessage(int sock){
	char optionBuffer[OPTIONSIZE]; 
	char receivedBuffer[RCVBUFSIZE + 1]; /* buffer for received string*/
    char userName[USERNAME_LENGTH];/* user name*/
    char message[RCVBUFSIZE + 1];
	int bytesRcvd;
/* Send the command option to the server*/
    optionBuffer[0] = '2';
    if (send(sock, optionBuffer, OPTIONSIZE, 0) != OPTIONSIZE)
        DieWithError("send() sent a different number of bytes than expected");

/* Receive up to the buffer size bytes from the sender */
    if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
    printf("%s" ,receivedBuffer); /* Print the buffer */

/* Send the username of receiver to server */
    scanf("%s", userName);
    fgetc(stdin); /* Capture '/n' */
    if (send(sock, userName, strlen(userName), 0) != strlen(userName))
        DieWithError("send() sent a different number of bytes than expected");

/* Receive up to the buffer size bytes from the sender */
    if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
    printf("%s" ,receivedBuffer); /* Print the buffer */

/* Send the message to server */
    fgets(message, sizeof(message), stdin); /* Get user's message */
    if (send(sock, message, strlen(message), 0) != strlen(message))
        DieWithError("send() sent a different number of bytes than expected");
/* Receive up to the buffer size bytes from the sender */
    if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
    printf("%s" ,receivedBuffer); /* Print the buffer */


}