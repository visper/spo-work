/*
Процесс 1 порождает потомка 2, они присоединяют к себе разделяемую память
объемом (N*sizeof(int)). Процесс 1 пишет в нее k1 чисел сразу,
процесс 2 переписывает k2 чисел из памяти в файл.
Процесс 1 может производить запись, только если в памяти достаточно места,
а процесс 2 переписывать, только если имеется не меньше, чем k2 чисел.
После каждой записи (чтения) процессы засыпают на t секунд.
После каждой записи процесс 1 увеличивает значение записываемых чисел на 1.
Используя семафоры, обеспечить синхронизацию работы процессов в соответствии
с заданными условиями: 18 вариант: N=10, k1=10, k2=5, t=1c.
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h> // sleep

#include <sys/sem.h>
#include <mem.h>

#define KEY          9001
#define SIZE         40//10//(N)
#define SLEEP_TIME   1
#define K1           10
#define K2           5

#define TIMEOUT 20 // for detete processes

int f3(int signum)
{
	printf("End of jobs");
	P(unlock_semaphore);
	delete_mem(parameters);
	kill(0, SIGKILL);
	return 0;
}

//function for realization operation by semaphore
//sid - identificator op - operation
static void semcall(int sid ,int op)
{
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op  = op;
	sb.sem_flg = 0;
	int check = semop(sid, &sb, 1);
	if(check != 0) {
		perror("error semop");
	}
}

//semaphore malloc 1
void P(int sid)
{
	semcall(sid, -1);
}

//semaphore free 0
void V(int sid)
{
	semcall(sid, 1);
}

void delete_mem(int memid, int semid)
{
	shmctl(memid, IPC_RMID, 0);
	shmctl(semid, 1, IPC_RMID, 0);
}

int main(int argc, char** argv)
{
	setpgrp();//for delete mem

	//stuct for save numbers
	struct numbers {
		int num1;
		int num2;
		int num3;
		int num4;
		int num5;
		int num6;
		int num7;
		int num8;
		int num9;
		int num10;
	};

	// id of mem
	int memid = shmget((key_t)KEY, SIZE, 0777 | IPC_CREAT);
	if(memid == -1) {
		perror("error memid alloc")
			return -1;
	}

	//take id of key of semapfore
	int semid = semget((key_t)KEY, 1, 0);

	int pid1 = 0, pid2 = 0;
	pid1 = getpid();
	if((pid2 = fork()) == 0) { //child
		pid2 = getpid();

		//connect mem to process
		char *addr = (char*)shmat(memid, 0, 0);
		int *pint = (int*) addr;

		while(1) {
			
			pint = (int*)addr;

		}


		sleep(1);
	}
	else if(pid2 > 0) { // parent

		//first initialization
		struct numbers n;
		n.num1 = 1;
		n.num2 = 10;
		n.num3 = 100;
		n.num4 = 1000;
		n.num5 = 10000;
		n.num6 = -20000;
		n.num7 = -2000;
		n.num8 = -200;
		n.num9 = -20;
		n.num10 = -2;

		//connect mem to process
		char *addr;
		addr = (char*)shmat(memid, 0, 0);
		int *pint;
		pint = (int*)addr;
		pint = n;
		pint = (int*)addr;

		//loop for increment
		for(size_t i = 0; i < K1; ++i) {
			num1++;
			num2++;
			num3++;
			num4++;
			num5++;
			num6++;
			num7++;
			num8++;
			num9++;
			num10++;
		}
		signal(SIGALRM, f3);
		alarm(TIMEOUT);

		sleep(1);
	}
	else {
		perror("fork error");
		return -1;
	}

	return 0;
}
