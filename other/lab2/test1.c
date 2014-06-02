#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define TIMEOUT 10

extern int f1(int), f2(int), f3(int);
int pid0, pid1, pid2;

FILE *file_open;

//int pid1 = 0, pid2 = 0; // pid processes
int end = 0;// = 1 if char = EOF
int N = 0;//number of chars for print

//fuu for read bytes from file_open
void read_file()
{
	static int ch;
	ch = fgetc(file_open);
	if(ch == EOF) {
		//send signal to proc about end of file
		fclose(file_open);//close file
		printf("\n");
		end = 1;
	}
	else {
		printf("%c", ch);
	}
	//here may be error because not check error report of fgetc
}

int main(int argc, char** argv)
{
	//argc must be equal 2 because argv[2] = name of file
	if(argc != 2) {
		printf("wrong input data!\n");
		return -1;
	}

	N = atoi(argv[1]);

	//check open file
	file_open = fopen("input", "r");
	if(!file_open) {
		printf("error input file!\n");
		return -1;
	}

	setpgrp();
	pid0 = getpid();
	printf("Start of job - %d\n", pid0);
	pid1 = fork();
	if (pid1 == 0) {/* process 1 */
		signal(SIGUSR1, f1);
		pid1 = getpid();
		pid2 = fork();
		if (pid2 < 0) puts("Fork error");
		if (pid2 > 0) for(;;);
		else {/* process 2 */
			signal(SIGUSR2, f2);
			pid2 = getpid();
			kill(pid1,SIGUSR1);
			for (;;);
		}
	}
	else {/* process 0 */
		signal(SIGALRM, f3);
		alarm(TIMEOUT);
		pause();
	}
	exit(0);
}

int f1(int signum)
{
	signal(SIGUSR1, f1);
	for(int i = 0; i < N; ++i) {
		read_file();
		if(end == 1) break;//EOF
	}

	if(end == 1) {
		//send signal SIG2
		if(getpid() == 0) { 
			int temp = getppid();
			assert(temp == pid1);
			kill(temp, SIGUSR2);
		}
		else {
			int temp = getpid();
			assert(temp == pid2);
			kill(temp, SIGUSR2)
		}
	}
	else {
		//send signal SIG1
		if(getpid() == 0) { 
			int temp = getppid();
			assert(temp == pid1);
			kill(temp, SIGUSR1);
		}
		else {
			int temp = getpid();
			assert(temp == pid2);
			kill(temp, SIGUSR1)
		}

	printf("Process 1 (%d) has got a signal from process 2 (%d)\n",
	       pid1,pid2);
	sleep(2);
	kill(pid2, SIGUSR2);
	return 0;
}

int f2(int signum)
{
	signal(SIGUSR2, f2);
	printf("Process 2 (%d) has got a signal from process 1 (%d)\n", 
	       pid2,pid1);
	sleep(2);
	kill(pid1, SIGUSR1);
	return 0;
}

int f3(int signum)
{
	printf("End of job - %d\n", pid0);
	kill(0, SIGKILL);
	return 0;
}


