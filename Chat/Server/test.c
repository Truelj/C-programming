#include <stdio.h>
#define MAX_USER 2 /* Maintains a parts database (array version)*/
#define NAME_LEN 25
#define PASSWORD_LEN 10
struct user{
    char name[NAME_LEN + 1];
    char password[PASSWORD_LEN + 1];
} usrlist[MAX_USER] = {
    {"Alice", "12345"},
    {"Bob", "56789"}
};

void printUser(){
	int i = 0;
	printf("print the user list\n");
	while(i < MAX_USER){
		printf("user name: %s ", usrlist[i].name);
		printf("password: %s \n", usrlist[i].password);
		i++;

	}
}