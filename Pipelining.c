#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int summation(int start, int end)
{
	int sum = 0;
	if (start < end)
	{
		sum = ((end * (end + 1)) - (start * (start - 1))) / 2;
	}
	return sum;
}

int ith_part_start(int i, int N, int M)
{
	int part_size = N / M;
	int start = i * part_size;
	return start;
}
int ith_part_end(int i, int N, int M)
{
	int part_size = N / M;
	int end = (i < M - 1) ? ((i + 1) * part_size - 1) : N;
	return end;
}
int main(int argc, char **argv)
{
	int port[2];
	pid_t pid;
	int i, j, k, l;
	int partial, sum;
	sum = 0;

	i = getpid();
	printf("parent(PID %d): process started\n\n", i);
	printf("parent(PID %d): forking child_1\n", i);
	pid = fork();

	if (pid < 0) {
		perror("fork error");
		exit(0);
	}

	if (pid > 0) //executed by parent
	{
		printf("parent(PID %d): fork successful for child_1(PID %d)\n", i, pid);
		printf("parent(PID %d): waiting for child_1(PID %d) to complete\n\n", i, pid);
		wait(NULL);
		printf("parent(PID %d): parent completed\n", i);
	}

	if (pid == 0) { //executed by child
		i = getpid();
		j = getppid();
		printf("child_1(PID %d): process started from parent(PID %d)\n", i, j);
		if (pipe(port) < 0) { //creates the pipe
			perror("pipe error");
			exit(0);
		}

		printf("child_1(PID %d): forking child_1.1....child_1.%d\n\n", i, atoi(argv[2]));
		for (k = 0; k < atoi(argv[2]); k++) { //collect partial sums and store them in pipe
			l = k + 1;
			pid = fork();
			wait(NULL);
			if (pid < 0) {
				perror("fork error");
				exit(0);
			}
			if (pid == 0) { //executed by grandchild process
				i = getpid();
				printf("child_1.%d(PID %d): fork() successful\n", l, i);
				int start = ith_part_start(k, atoi(argv[1]), atoi(argv[2]));
				int end = ith_part_end(k, atoi(argv[1]), atoi(argv[2]));
				int result = summation(start, end);
				printf("child_1.%d(PID %d): partial sum: [%d - %d] = %d\n", l, i, start, end, result);
				write(port[1], &result, sizeof(int));
				return 0;
			}
		}

		for (int k = 0; k < atoi(argv[2]); k++) { //read partial sums from pipe and calculate sum
			read(port[0], &partial, sizeof(int));
			sum = sum + partial;
		}
		printf("\nchild_1(PID %d): total sum = %d\n", i, sum);
		printf("child_1(PID %d): child_1 completed\n\n", i);
		return 0;
	}

	return 0;
}