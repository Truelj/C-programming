#include <stdio.h>
#include <string.h>

int main(){
	char *welcome = "welcome! \nusername:";
	printf("%s", welcome);
	printf("\n");
	printf("length %lu", strlen(welcome));

	char username[20];
	printf("username: ");
	scanf("%s", username);
	printf("strlen: %lu", strlen(username));

	char correct_username[20];
	strcpy(correct_username, username);
	printf("correct_username %s\n", correct_username);

	return 0;
}