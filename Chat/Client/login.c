#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define RCVBUFSIZE 5000 /* Size of receive buffer */
#define OPTIONSIZE 1 /* Size of option buffer*/

#define USERNAME_LENGTH 20
#define PASSWORD_LENGTH 20
void DieWithError(char *errorMessage); /* Error handling function */

/* log into the server*/
void login(int sock, char *user){
    char userName[USERNAME_LENGTH];/* user name*/
    char password[PASSWORD_LENGTH];/*user password*/

    char receivedBuffer[RCVBUFSIZE + 1]; /* buffer for received string*/
    char expect_one[RCVBUFSIZE + 1];
    char expect_two[RCVBUFSIZE + 1];
    int bytesRcvd, totalBytesRcvd; /* Bytes read in single recv() and total bytes read */

    char optionBuffer[OPTIONSIZE]; 

/*send the command option to the server*/
    optionBuffer[0] = '0';
    if (send(sock, optionBuffer, OPTIONSIZE, 0) != OPTIONSIZE)
        DieWithError("send() sent a different number of bytes than expected");

/* Expects to receive "Welcome! Please login \nusername: "*/
/* Receive up to the buffer size bytes from the sender */
    if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
    printf("%s" ,receivedBuffer); /* Print the buffer */

/* Send the user name to the server */
    strcpy(expect_one, "password: ");
    while(strcmp(expect_one, receivedBuffer) != 0){
        /* send the username to the server */
        scanf("%s", userName);
        if(send(sock, userName, strlen(userName), 0) != strlen(userName))
            DieWithError("send() sent a different number of bytes than expected");
        /* Expects to receive "password: "*/
        memset(receivedBuffer, 0, sizeof receivedBuffer);
        if((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
        printf("%s" ,receivedBuffer); /* Print the buffer */

    }
/* Send the password to the server */
    strcpy(expect_two, "log in success\n");
    while(strcmp(expect_two, receivedBuffer) != 0){
        /* Send the password to the server */
        scanf("%s", password);
        if(send(sock, password, strlen(password), 0) != strlen(password))
            DieWithError("send() sent a different number of bytes than expected");
        /* Expects to receive "log in successs" */
        memset(receivedBuffer, 0, sizeof receivedBuffer);
        if((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
        printf("%s" ,receivedBuffer); /* Print the buffer */

    }
    
    strcpy(user, userName);

}
