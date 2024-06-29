#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

int numArray[4];

void* sum(void *thread_id) {
	long unsigned int tid = *(long unsigned int*)thread_id;
	printf("thread_1 (TID %lu) reads X = %d and Y = %d from input_array[]\n", tid, numArray[0], numArray[1]);
	int result = numArray[0] + numArray[1];
	printf("thread_1 (TID %lu) writes X + Y = %d to the input_array[2]\n", tid, result);
	numArray[2] = result;
}

void* even_odd(void* thread_id) {
	long unsigned int tid = *(long unsigned int*)thread_id;
	if (numArray[2] % 2 == 0) {
		printf("thread_2 (TID %lu) identifies that %d is an even number\n", tid, numArray[2]);
	}
	else {
		printf("thread_2 (TID %lu) identifies that %d is an odd number\n", tid, numArray[2]);
	}
}

void* multiplication(void* thread_id) {
	long unsigned int tid = *(long unsigned int*)thread_id;
	int result = numArray[0] * numArray[1];
	printf("thread_2 (TID %lu) reads X and Y from input_array[],writes X * Y = %d to input_array[3]\n", tid, result);
	numArray[3] = result;
}

void* thread_2_tasks(void *thread_id) {
	long unsigned int tid = *(long unsigned int*)thread_id;
	void* function;
	printf("thread_2 (TID %lu) reads %d from the input_array[2]\n", tid, numArray[2]);
	function = even_odd(thread_id);
	function = multiplication(thread_id);
}

void* reverse_num(void *thread_id) {
	long unsigned int tid = *(long unsigned int*)thread_id;
	int remainder;
	int revNum = 0;
	int i = numArray[3];
	printf("thread_3 (TID %lu) reads %d from input_array[3]\n", tid, numArray[3]);
	while (i != 0) {
		remainder = i % 10;
		revNum = revNum * 10 + remainder;
		i = i /= 10;
	}
	printf("thread_3 (TID %lu) reverses the number %d -> %d\n", tid, numArray[3], revNum);
}

int main(int argc, char** argv) {

	pthread_t thread_1,thread_2, thread_3;

	pid_t pid;
	pid = getpid();
	printf("parent (PID %d) receives X = %d and Y = %d from the user\n", pid, atoi(argv[1]), atoi(argv[2]));
	printf("parent (PID %d) writes X = %d and Y = %d to input_array[]\n", pid, atoi(argv[1]), atoi(argv[2]));
	numArray[0] = atoi(argv[1]);
	numArray[1] = atoi(argv[2]);

	pthread_create(&thread_1, NULL, sum, &thread_1);
	pthread_join(thread_1, NULL);

	pthread_create(&thread_2, NULL, thread_2_tasks, &thread_2);
	pthread_join(thread_2, NULL);

	pthread_create(&thread_3, NULL, reverse_num, &thread_3);
	pthread_join(thread_3, NULL);
}