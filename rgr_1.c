/*
bugs
-problem with exec function of p4, it run aftes some time after 
 run program
-proces for counting work with error numbers
 maybe problem with read it from memory
-printf of "p" function = 0

 */
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

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define KEY 1111

FILE *file1;
FILE *file2;
FILE *file3;
FILE *file4;

int pid1, pid3, pid4, pid2;

int shmid = 0;
//char *shm;
int *shm;

int p1(int signum)
{

	printf("TEST P1\n");
	//read from console
	//this is B
//	signal(SIGUSR1, p1);

	//read from console
	printf("write number B\n");

	int a;
	scanf("%i", &a);

/*
	printf("shm before chage = 0x%x\n", shm);
	int *temp = 0;
	temp = shm;
	temp = temp + sizeof(int); //adress+offset
	shm = temp;
	*shm = a;
	printf("shm after chage =  0x%x\n", shm);
	printf("sizeof(int) = %i__________\n", sizeof(int));
*/
	shm[1] = a;

	//maybe it`s enought_________
	//returned adress
/*
	temp = shm;
	temp = temp - sizeof(int); //adress+offset
	shm = temp;
	//enought____________________
	temp = 0;
*/
//maybe we must to change adress of *shm back to
//temp - sizeof(int)

	//kill signal to C
	printf("pid4 = %i\n", pid4);

	sleep(1);
	kill(pid4, SIGUSR1);
	return 0;
}

//int p2(int signum)
int p2(void)
{
	printf("TEST P___2\n");
	//read from file
	//this is A
//	signal(SIGUSR1, p2);
	//open file
	file2 = fopen("file2", "r");
	if(!file2) {
		printf("error input file2!\n");
		return -1;
	}
	//read from file, this is A and we must not
	//do the offset
	//	fscanf(file2, "%i", *shm);
	int tmp;
	fscanf(file2, "%i", &tmp);

	printf("test %i\n", tmp);
	*shm = tmp;

	fclose(file2);//close file
	//kill signal to B
//	printf("pid1 = %i\n", pid1);
	sleep(1);

	printf("proc 2 already fone\n");

	kill(pid1, SIGUSR1);
	return 0;
}

int p3(int signum)
{
	printf("TEST P_________3\n");
	//wait for numbers, compute (A+B)*C
	//and write to console
//	signal(SIGUSR1, p3);
	int a = *shm;
//	int b = shm + sizeof(int);
	int b = shm[1];
//	int c = shm + (2*sizeof(int));
	int c = shm[2];

	printf("a = %i\n", a);
	printf("b = %i\n", b);
	printf("c = %i\n", c);

	int result = (a + b) * c;

	printf("result = %i\n", result);

	sleep(1);
	kill(0, SIGKILL);
//	return 0;
		exit(0);
}

int p4(int signum)
{
	printf("TEST P_________________4\n");
	//read from file4
	//this is C
//	signal(SIGUSR1, p4);

	//open file
	file4 = fopen("file4", "r");
	if(!file4) {
		printf("error input file4!\n");
		return -1;
	}

	//read from file, do offset by 2*sizeof(int);
/*
	printf("shm before chage = 0x%x\n", shm);
	int *temp = 0;
	temp = shm;
	temp = temp +(2*sizeof(int));
	shm = temp;
	printf("shm after chage =  0x%x\n", shm);
	printf("sizeof(int) = %i__________\n", sizeof(int));
*/

	int t = 0;
	fscanf(file4, "%i", &t);

//	*shm = t;
	shm[2] = t;

//	printf("shm pid4 = %i\n", *shm);
	fclose(file4);//close file

	//maybe it`s enought_________
	//returned adress
/*
	temp = shm;
	temp = temp - (2*sizeof(int)); //adress-offset
	shm = temp;
*/
	//enought____________________

//	temp = 0;
	//maybe we must change adress of shm by
	//shm - (2*sizeof(int))

	//kill signal to pid3

	printf("pid3 = %i\n", pid3);

	sleep(1);
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

	struct mem {
		int A;
		int B;
		int C;
	};
//	} *ptr;
	
	//this struct will be use as common resource 12 bytes
//	struct mem *ptr;
//	ptr = (struct mem*)malloc(sizeof(strucr mem));

	//we must create the segment



	struct mem ptr;

	size_t ptr_len = sizeof(ptr);
	printf("len of allocate memory %zu\n", ptr_len);

	
	// Create the segment and set permissions.
	if ((shmid = shmget((key_t)KEY, ptr_len, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		return -1;
	}	


	// Now we attach the segment to our data space.
//	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
	if ((shm = shmat(shmid, 0, 0)) == (char *) -1) {
		perror("shmat");
		return 1;
	}

	// Zero out memory segment initialize
	memset(shm,0,ptr_len);
//	s = shm;

	pid1 = getpid();//parent
	pid3 = 0;
	pid4 = 0;
	pid2 = fork();

	if(pid2 == 0) { //child  proc2
		pid2 = getpid();
		printf("child\n");
//		kill(pid1, SIGUSR1);

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
