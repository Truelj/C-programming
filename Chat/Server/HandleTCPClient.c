#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h> /* for close() */
#include <string.h> /* for strlen()*/
#include <stdlib.h>
#include <stdbool.h> /* for boolean type */

#define RCVBUFSIZE 50 /* Size of receive buffer */
#define OPTIONSIZE 1 /* Size of option buffer */

#define MAX_USER 2 /* Maintains a parts database (array version)*/
#define NAME_LEN 25
#define PASSWORD_LEN 10
struct user{
    char name[NAME_LEN + 1];
    char password[PASSWORD_LEN + 1];
    char message[RCVBUFSIZE + 1];
} usrlist[MAX_USER] = {
    {"Alice", "12345"},
    {"Bob", "56789"}
};
int currentUserId;

void DieWithError(char *errorMessage); /* Error handling function */
void logIn(int clntSocket);
void returnUserList(int clntSocket);
void recordUserMessage(int clntSocket);
void disconnect(int clntSocket);
void returnMessage(int clntSocket);

void HandleTCPClient(int clntSocket){
    char optionBuffer[OPTIONSIZE]; /* Buffer for option */
    int recvMsgSize; /* Size of received message */
    int option;
    bool connected = true;

    while(connected){
            /* Receive option from client */
        if ((recvMsgSize = recv(clntSocket, optionBuffer, OPTIONSIZE, 0)) < 0)
           DieWithError("recv() failed") ;
        /* Convert the option string into option number */
        option = atoi(optionBuffer);
        switch(option){
            case 0: logIn(clntSocket); break;
            case 1: returnUserList(clntSocket); break;
            case 2: recordUserMessage(clntSocket);break;
            case 3: returnMessage(clntSocket);break;
            case 4: disconnect(clntSocket); connected = false; break;
            case 5: disconnect(clntSocket); connected = false; break;
            default: break;
        }
    }
}

void returnMessage(int clntSocket){
    char message[RCVBUFSIZE];
    char sendBuffer[RCVBUFSIZE];
    strcpy(message, usrlist[currentUserId].message);
    /* Send user message */
    if(send(clntSocket, message, strlen(message), 0) != strlen(message))
        DieWithError("send() failed");
    printf("Send back %s's message!\n", usrlist[currentUserId].name);
}

void disconnect(int clntSocket){
    char *one = "-----------disconnect with the server-----------\n";
    /* Send user message */
    if(send(clntSocket, one, strlen(one), 0) != strlen(one))
        DieWithError("send() failed");

    close(clntSocket);
    printf("Client disconnected \n");
}

void recordUserMessage(int clntSocket){
    char name[NAME_LEN + 1]; /* stores the name of message receiver*/
    char message[RCVBUFSIZE]; /* Buffer for receiving message */
    int recvMsgSize; /* Size of received message */
    char *one = "Please enter the user name: ";
    char *two = "Please enter the message: ";
    char *three = "message sent successfully!\n";
    int i =0;
/* Send user message one*/
    if(send(clntSocket, one, strlen(one), 0) != strlen(one))
        DieWithError("send() failed");
    /*printf("successfully sent message one\n");*/

/* Receive username from the client */
    if((recvMsgSize = recv(clntSocket, name, NAME_LEN, 0)) < 0)
        DieWithError("recv() failed") ;
    name[recvMsgSize] = '\0'; /* Terminate the string! */
    /*printf("successfully receive the user name : %s\n", name);*/
/* Find the user from database */
    while (i < MAX_USER){
        if(strcmp(usrlist[i].name, name) == 0)/* find the user in database */
            break;
        i++;
    }
/* Send user message two*/
    if(send(clntSocket, two, strlen(two), 0) != strlen(two))
        DieWithError("send() failed");
    /*printf("successfully sent message two\n");*/

/* Receive message from the client */
    if((recvMsgSize = recv(clntSocket, message, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed") ;
    message[recvMsgSize] = '\0'; /* Terminate the string! */
/* Store the message to the database */
    strcpy(usrlist[i].message, message);
    /*printf("successfully store the message %s", usrlist[i].message);*/

/* Send user message three*/
    if(send(clntSocket, three, strlen(three), 0) != strlen(three))
        DieWithError("send() failed");
    /*printf("successfully sent message three\n");*/
    printf("A message to %s \n", usrlist[i].name);
    printf("%s", usrlist[i].message);
}

void returnUserList(int clntSocket){
    char receivedBuffer[RCVBUFSIZE]; /* Buffer for receiving message */
    int recvMsgSize; /* Size of received message */
    char *userList = "There are 2 users:\nAlice\nBob\n";
/* Send user list back to client */
    if(send(clntSocket, userList, strlen(userList), 0) != strlen(userList))
        DieWithError("send() failed");
    printf("Return user list\n");
}

void logIn(clntSocket){
    char receivedBuffer[RCVBUFSIZE]; /* Buffer for receiving message */
    int recvMsgSize; /* Size of received message */
    char *one = "Welcome! Please log in \nusername: ";
    char *two = "password: ";
    char *three = "log in success\n";
    char *user_exception = "user not found \nusername: ";
    char *password_exception ="password is not corrcet \npassword: ";
    char name[NAME_LEN + 1];
    char password[PASSWORD_LEN + 1];
    char correct_password[PASSWORD_LEN+1];
    int i = 0; /* used to find the user ID in database */
    bool found = false;
    /* Ask for user name */
    if(send(clntSocket, one, strlen(one), 0) != strlen(one))
        DieWithError("send() failed");
    while(!found){
        /* Receive username from the client */
        if((recvMsgSize = recv(clntSocket, name, NAME_LEN, 0)) < 0)
            DieWithError("recv() failed") ;
        name[recvMsgSize] = '\0'; /* Terminate the string! */
        /* Check the user */
        while (i < MAX_USER){
            if(strcmp(usrlist[i].name, name) == 0){
                printf("log in user name is %s\n", name);
                found = true; /* find the user id in database */
                currentUserId = i;
                strcpy(correct_password, usrlist[i].password);/* get the password for the user */
                break;
            }
            i++;
        }
        if(i == MAX_USER){/* Did not find the user in database */
            printf("user not found \n");
            /* resend the message */
            if(send(clntSocket, user_exception, strlen(user_exception), 0) != strlen(user_exception))
                DieWithError("send() failed");
        }
        i = 0;
        memset(name, 0, sizeof name);
    }
    /* Ask for password */
    if(send(clntSocket, two, strlen(two), 0) != strlen(two))
        DieWithError("send() failed");
    /* Receive password from the client */
    if((recvMsgSize =recv(clntSocket, password, PASSWORD_LEN, 0)) < 0)
        DieWithError("recv() failed") ;
    password[recvMsgSize] = '\0'; /* Terminate the string! */
    printf("log in password is %s\n", password);
    /* Check the password*/
    while(strcmp(correct_password, password) != 0){
        printf("password not correct\n");
        /* Ask for password */
        if(send(clntSocket, password_exception, strlen(password_exception), 0) != strlen(password_exception))
            DieWithError("send() failed");
        memset(password, 0, sizeof password);
        /* Receive password from the client */
        if((recvMsgSize =recv(clntSocket, password, PASSWORD_LEN, 0)) < 0)
             DieWithError("recv() failed") ;
        password[recvMsgSize] = '\0'; /* Terminate the string! */
        printf("log in password is %s\n", password);
    }
    /* Send message back to client */
    if(send(clntSocket, three, strlen(three), 0) != strlen(three))
        DieWithError("send() failed");
}
