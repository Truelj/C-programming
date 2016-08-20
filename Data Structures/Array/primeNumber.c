/* Author: Jie Li*/
/* Date: Aug 18, 2016*/
/* The goal of this program is to set a[i] to 1 if i is prime, and to 0 if i is not prime */
/* Print all primes numbers that are less than 1000 at the end of the program */
#include <stdio.h>

#define N 10000
int main(){
	int a[N];
	for(int i = 0; i < N; i++){
		a[i] = 1;
	}
	/* 0 and 1 are  prime*/
	for(int i = 2; i < N; i++){
		if(a[i]){
			for(int j = i; j * i < N; j++){
				a[i * j] = 0;
			}
		}
	}
	/* print all prime numbers less than N */
	for(int i = 0; i < N; i++){
		if(a[i]){
			printf("%d \n", i);
		}
	}

	return 0;
}