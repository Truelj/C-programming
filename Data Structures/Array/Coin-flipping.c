/* Author: Jie Li*/
/* Date: Aug 18, 2016*/
/* If we flip a coin N times, we expect ot get N/2 heads, but could get anywhere from 0*/
/* to N heads. This program runs the experiment M times, taking both N and M from the  */
/* command line */

#include <stdio.h>
#include <stdlib.h>

int heads() {
	return rand() < RAND_MAX/2;
}

int main(int argc, char *argv[]){
	int N = atoi(argv[1]), M = atoi (argv[2]);
	int *f = malloc((N+1)*sizeof (int));
	int count;
	for(int i = 0; i <= N; i++){
		f[i] = 0;
	}
	for(int i = 0; i < M; i++){
		count = 0;
		for(int j = 0; j < N; j++){
			if(heads()){
				count++;
			}
		}
		f[count]++;
	}
	for(int i = 0; i <= N; i++){
		printf("%2d ", i);
		for(int j = f[i]; j > 0; j -= 10){
			printf("*");
		}
		printf("\n");
	}
	return 0;
}