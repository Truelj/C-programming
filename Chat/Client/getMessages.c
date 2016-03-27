#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <string.h> 

#define OPTIONSIZE 1 /* Size of option buffer*/
#define RCVBUFSIZE 5000 /* Size of receive buffer */
void DieWithError(char *errorMessage); /* Error handling function */

void getMyMessages(int sock){
	char optionBuffer[OPTIONSIZE]; 
	char receivedBuffer[RCVBUFSIZE + 1]; /* buffer for received string*/
	int bytesRcvd;
/* Send the command option to the server*/
    optionBuffer[0] = '3';
    if (send(sock, optionBuffer, OPTIONSIZE, 0) != OPTIONSIZE)
        DieWithError("send() sent a different number of bytes than expected");

/* Receive up to the buffer size bytes from the sender */
    if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
    printf("%s" ,receivedBuffer); /* Print the message, which ends with '/n'*/

}