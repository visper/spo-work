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
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

FILE *file_open;

int N = 0;

//fuu for read bytes from file_open
int read_file()
{
	static int ch;
	ch = fgetc(file_open);
//	if(ch == EOF) {
//	if(fgetc(file_open) == -1) {
	if(ch == EOF) {
		//send signal to proc about end of file
		fclose(file_open);//close file
//		printf("\n");
		return -1;
	}
	else {
		printf("%c", ch);
		return 0;
	}
	//here may be error because not check error report of fgetc
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

	while(read_file() == 0);

	return 0;
}
