/*
Процесс 1 открывает существующий файл и порождает потомка 2.
Процесс 1 считывает N байт из файла, 
выводит их на экран и посылает сигнал SIG1 процессу 2.
Процесс 2 также считывает N байт из файла, 
выводит их на экран и посылает сигнал SIG1 процессу 1. 
Если одному из процессов встретился
конец файла, то он посылает другому процессу сигнал SIG2 и они оба завершаются.
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <wchar.h>
#include <wctype.h>

FILE *file_open;

int pid1 = 0, pid2 = 0; // pid processes
int N = 0;//number of chars for print

//fuu for read bytes from file_open
void read_file()
{
//	static wchar_t ch;
//	static char ch;
	static int ch;
	ch = fgetc(file_open);

	//send signal to proc about end of file
	//	if(ch == EOF) {
	if(ch == '!') {
		fclose(file_open);//close file
//		printf("\n");
//		printf("int EOF %i\n", EOF);
		//change to kill to 2 process not "0"
/*
		if(getpid() == pid1) { //parrent
			kill(pid2, SIGUSR2);
		}
		else { //child
			kill(pid1, SIGUSR2);
		}
*/
		kill(0, SIGUSR2);
	}
	else {
//		printf("test\n");
		printf("%c", ch);
	}
	//here may be error because not check error report of fgetc
}

int print(int signum)
{
	signal(SIGUSR1, print);

	for(int i = 0; i < N; ++i) {
		read_file();
	}
/*
	printf("pid1 = %i\n", pid1);
	printf("pid2 = %i\n", pid2);
	printf("%i\n", getpid());
*/
	//send signal SIG1
	if(getpid() == pid1) { //parrent
//		printf("test__ for pid 1\n");
		kill(pid2, SIGUSR1);
	}
	else { //child
//		printf("test__ for pid 2\n");
		kill(pid1, SIGUSR1);
	}
	return 0;
}

int end(int signum)
{
//	signal(SIGUSR2, end);
	//send signal
//	printf("end\n");
	fclose(file_open);//close file
	kill(0, SIGKILL);

	return 0;
}

//argv[1] - N bytes for read
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

	setpgrp();//hz
	signal(SIGUSR1, print);
	signal(SIGUSR2, end);

	pid1 = getpid();
	pid2 = fork();

	if(pid2 == 0) { //child
		pid2 = getpid();
		printf("child\n");
		kill(pid1, SIGUSR1);
		for(;;);
	}
	else { //parent
		printf("parent\n");
		for(;;);
	}

	return 0;
}
