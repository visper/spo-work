/*
  На рис.1 показана общая схема взаимодействия 4-х процессов. Процесс 1 является
  предком процессов 2 и 4. Процесс 3 потомком Процесса 2. Процессы взаимодействуют
  друг с другом через общий ресурс (разделяемую память, именованный программный
  канал, очередь сообщений, регулярный файл). Каждый из процессов осуществляет ввод-
  вывод в стандартный поток или файл.
  Синхронизация выполнения процессов осуществляется с помощью системных вызовов,
  сигналов и семафоров. К каждому файлу (1...4) имеет непосредственный доступ только
  один процесс, создавший его.
  Процессы взаимодействуют следующим образом: три процесса источника передают
  параллельно через общий ресурс по одному целому числу A, B, C, полученному с
  клавиатуры или из входного файла, процесс приемник выполняет над полученными
  числами операцию сложения и умножения: (A+B)*C и выводит результат на экран или файл.
  Необходимо реализовать синхронизированный обмен между процессами с выводом
  результата взаимодействия между ними, согласно предлагаемым вариантам задания
  (Таблица 1.)
  Источник А процесс 2, файл 2
  Источник В процесс 1, консоль
  Источник C процесс 4, файл 4
  Общий ресурс память
  Приемник/Результат процес 3, консоль
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

FILE *file1;
FILE *file2;
FILE *file3;
FILE *file4;

struct mem {
	int A;
	int B;
	int C;
} *ptr;

int pid1, pid3, pid4, pid2;

int p1(int signum)
{
	//read from console
	//this is B
//	signal(SIGUSR1, p1);

	//read from console
	printf("write number B\n");
//	scanf("%i", ptr->B);
	int a;
	scanf("%i", &a);
	*ptr->B = a;

	//kill signal to C

	kill(pid4, SIGUSR1);
	return 0;
}

//int p2(int signum)
int p2(void)
{
	//read from file
	//this is A
//	signal(SIGUSR1, p2);
	//open file
	file2 = fopen("file2", "r");
	if(!file2) {
		printf("error input file2!\n");
		return -1;
	}
	//read from file
	fscanf(file2, "%i", ptr->A);
//	fscanf(file2, "%i", &(*ptr->A));			
	fclose(file2);//close file

	//kill signal to B

	kill(pid1, SIGUSR1);
	return 0;
}

int p3(int signum)
{
	//wait for numbers, compute (A+B)*C
	//and write to console
//	signal(SIGUSR1, p3);

	int result = ((ptr->A) + (ptr->B)) * (ptr->C);

	printf("result = %i\n", result);


	kill(0, SIGKILL);
//	return 0;
		exit(0);
}

int p4(int signum)
{
	//read from file4
	//this is C
//	signal(SIGUSR1, p4);

	//open file
	file4 = fopen("file4", "r");
	if(!file4) {
		printf("error input file4!\n");
		return -1;
	}

	//read from file
	fscanf(file4, "%i", ptr->C);
//	fscanf(file4, "%i", &(*ptr->C));			
	fclose(file4);//close file

	//kill signal to pid3
	kill(pid3, SIGUSR1);

	return 0;
}

//void program(void)
//{

//}

int main(int argc, char** argv)
{
	printf("init\n");

	signal(SIGUSR1, p1);

	setpgrp();
//	signal(SIGUSR1, print);
//	signal(SIGUSR2, end);

	//this struct will be use as common resource 12 bytes
//	struct mem *ptr;
//	ptr = (struct mem*)malloc(sizeof(strucr mem));

	pid1 = getpid();//parent
	pid3 = 0;
	pid4 = 0;
	pid2 = fork();

	if(pid2 == 0) { //child  proc2
		pid2 = getpid();
		printf("child\n");
		kill(pid1, SIGUSR1);

		pid3 = fork();
		if(pid3 == 0) { //child proc 3
			pid3 = getpid();
			signal(SIGUSR1, p3);
			while(1){
			//wait for numbers, compute (A+B)*C
			//and write to console

				pause();
			}
		}
		else { //child proc 2
			//read from file
			//this is A
		  //			while(1) {
		  //				signal(SIGUSR1, p2);
		  //		pause();
				//			}
			p2();
		}

	}
	else { //parent proc1
		pid4 = fork();
		if(pid4 == 0) { //child proc4
			pid4 = getpid();
			signal(SIGUSR1, p4);
			//read from file4
			//this is C
			while(1) {
				pause();
			}

		}
		else { //parent proc1
			//read from console
			//this is B
			while(1) {
				pause();
			}

		}

		printf("parent\n");

	}





//	program();


	return 0;
}
