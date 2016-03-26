#include <stdio.h> 
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <string.h> /* for memset() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <unistd.h> /* for close() */

#define RCVBUFSIZE 50 /* Size of receive buffer */
#define IP_LENGTH 15 /* Maximum length of IP address */
#define MAXPENDING 5 /* Maximum outstanding connection requests */
#define OPTIONSIZE 1
#define USERNAME_LENGTH 20

void connectToServer();
void disconnectServer();
void login(int sock, char *user);
void getUserList(int sock);
void sendMessage(int sock);
void getMyMessages(int sock);
void initiateChat();
void chatWithFriend(int clntSock, char *friend_name, char *user);
void connectToFriend();
void chat(int sock, char *user);
void DieWithError(char *errorMessage); /* Error handling function */

int sock; /* Socket descriptor */
struct sockaddr_in serverAddr; /* server address */
char servIP[IP_LENGTH]; /* Server IP address (dotted quad) */
unsigned short serverPort; /* server port */
char user[USERNAME_LENGTH];/* current user's name*/

int main(int argc, char const *argv[])
{
	int option;
	while(1){
		printf("---------------------------------------\n");
		printf("Command:\n");
		printf("0. Connect to the server\n");
		printf("1. Get the user list\n");
		printf("2. Send a message\n");
		printf("3. Get my messages\n");
		printf("4. Initiate a chat with my friend\n");
		printf("5. Chat with my friend\n");
		printf("Your option <enter a number>:");
		scanf("%d", &option);
		/* According to the option the user entered, perform the corresponding task*/
		switch(option){
			case 0: connectToServer(); login(sock, user);break;
			case 1: getUserList(sock); break;
			case 2: sendMessage(sock); break;
			case 3: getMyMessages(sock); break;
			case 4: initiateChat(); break;
			case 5: connectToFriend();chat(sock, user); break;
			default: break;

		}

	}
	return 0;
}

void connectToFriend(){
	char optionBuffer[OPTIONSIZE]; 
    int bytesRcvd;
    char receivedBuffer[RCVBUFSIZE + 1];
/*send the command option to the server*/
    optionBuffer[0] = '5';
    if (send(sock, optionBuffer, OPTIONSIZE, 0) != OPTIONSIZE)
        DieWithError("send() sent a different number of bytes than expected");
/* Receive up to the buffer size bytes from the sender */
    if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
    printf("%s" ,receivedBuffer); /* Print the buffer */
/* Disconnect with server */
	disconnectServer();

/* Prompt the user to enter the IP address and port number of the friend */
	printf("Please enter your friend's IP address: ");
	scanf("%s", servIP);
	printf("Please enter your friend's port number: ");
	scanf("%hu", &serverPort);

/* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError(" socket () failed") ;


/* Construct the server address structure */
    memset(&serverAddr, 0, sizeof(serverAddr)); /* Zero out structure */
    serverAddr.sin_family = AF_INET; /* Internet address family */
    serverAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
    serverAddr.sin_port = htons(serverPort); /* Server port */

	printf("Connecting......\n");
/* Establish the connection to a friend */
    if (connect(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
        DieWithError(" connect () failed");

 	printf("Connected!\n");
}

void connectToServer(){
/* Prompt the user to enter the IP address and port number of server */
	printf("Please enter the IP address: ");
	scanf("%s", servIP);
	printf("Please enter the port number: ");
	scanf("%hu", &serverPort);

/* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError(" socket () failed") ;


/* Construct the server address structure */
    memset(&serverAddr, 0, sizeof(serverAddr)); /* Zero out structure */
    serverAddr.sin_family = AF_INET; /* Internet address family */
    serverAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
    serverAddr.sin_port = htons(serverPort); /* Server port */

	printf("Connecting......\n");
/* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
        DieWithError(" connect () failed");

 	printf("Connected!\n");
}

void initiateChat(){

	int servSock; /* Socket descriptor for server */
    int clntSock; /* Socket descriptor for client */
    struct sockaddr_in servAddr; /* Local address */
    struct sockaddr_in clntAddr; /* Client address */
    unsigned short servPort; /* Server port */
    unsigned int clntLen; /* Length of client address data structure */
    int clnt_number = 0;/* client number for each connected client*/
	char receivedBuffer[RCVBUFSIZE + 1]; 
	char friend_name[USERNAME_LENGTH];

    char optionBuffer[OPTIONSIZE]; 
    int bytesRcvd;
/*send the command option to the server*/
    optionBuffer[0] = '4';
    if (send(sock, optionBuffer, OPTIONSIZE, 0) != OPTIONSIZE)
        DieWithError("send() sent a different number of bytes than expected");
/*receive message from server*/
/* Receive up to the buffer size bytes from the sender */
    if ((bytesRcvd = recv(sock, receivedBuffer, RCVBUFSIZE, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    receivedBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
    printf("%s" ,receivedBuffer); /* Print the buffer */
    memset(receivedBuffer,0, sizeof receivedBuffer);
/* Disconnect with server */
	disconnectServer();
	printf("please enter the port number you want to listen on : ");
	scanf("%hu", &servPort);


/* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError( "socket () failed") ;

/* Construct local address structure */
    servAddr.sin_family = AF_INET; /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(servPort); /* Local port */

/* Bind to the local address */
    if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        DieWithError ( "bind () failed");
/* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed") ;

    printf("I am listening on 127.0.0.1 : %hu\n", servPort);

/* Set the size of the in-out parameter */
    clntLen = sizeof(clntAddr);
/* Wait for a client to connect */
    if ((clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntLen)) < 0)
        DieWithError("accept() failed");
/* clntSock is connected to a client! */
/* Receive the sender's name */
    if ((bytesRcvd = recv(clntSock, friend_name, USERNAME_LENGTH, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
   	friend_name[bytesRcvd] = '\0'; /* Terminate the string! */
    printf("%s is connected\n", friend_name);
    chatWithFriend(clntSock, friend_name, user);
    

}
void disconnectServer(){
	close(sock);
}

