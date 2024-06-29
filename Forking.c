#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	pid_t pid, i, j, pid1;

	pid = fork();

	if (pid < 0) {
		printf("First Fork unsuccessful!\n");
		return 0;
	}

	if (pid > 0) { //executed by parent
		i = getpid();
		printf("parent(PID %d): process started\n\n", i);
		printf("parent(PID %d): forking child_1\n", i);
		printf("parent(PID %d): fork successful for child_1(PID %d)\n", i, pid);
		printf("parent(PID %d): waiting for child_1(PID %d) to complete\n", i, pid);
	}

	if (pid == 0) { //executed by child_1
		i = getpid();
		j = getppid();
		printf("child_1(PID %d): process started from parent(PID %d)\n\n", i, j);
		printf("child_1(PID %d): forking child_1.1\n", i);
		pid1 = fork();
		wait(NULL);
		if (pid1 == 0) { //executed by child_1.1
			i = getpid();
			j = getppid();
			printf("child_1.1 (PID %d): process started from child_1 (PID %d)\n", i, j);
			printf("child_1.1 (PID %d): calling an external program [./external_program1.out]\n", i);
			execl("./external_program1.out", argv[1], NULL);
		}
		printf("child_1(PID %d): completed child_1.1\n\n", i);
		printf("child_1(PID %d): forking child_1.2\n", i);
		pid1 = fork();
		wait(NULL);
		if (pid1 == 0) { //executed by child_1.2
			i = getpid();
			j = getppid();
			printf("child_1.2 (PID %d): process started from child_1 (PID %d)\n", i, j);
			printf("child_1.2 (PID %d): calling an external program [./external_program1.out]\n", i);
			execl("./external_program1.out", argv[2], NULL);
		}
		printf("child_1(PID %d): completed child_1.2\n\n", i);
		return 0;
	}
	wait(NULL);

	printf("parent (PID %d): forking child_2\n", i);
	pid = fork();
	if (pid > 0) { //executed by parent
		printf("parent (PID %d): fork successful for child_2 (PID %d)\n", i, pid);
	}
	wait(NULL);

	if (pid == 0) { //executed by child_2
		i = getpid();
		j = getppid();
		printf("child_2 (PID %d): process started from parent (PID %d)\n", i, j);
		printf("child_2 (PID %d): calling an external program [./external_program2.out]\n", i);
		execl("./external_program2.out", argv[3], NULL);
	}

	printf("parent (PID %d): completed parent\n", i);
	return 0;
}
