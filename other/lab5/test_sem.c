#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>





static void semcall(int sid, int op)
{
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = op;
	sb.sem_flg = 0;
	semop(sid, &sb, 1);
}

void P(sid)
{
	semcall(sid, -1);
}

void V(sid)
{
	semcall(sid, 1);
}

int main()
{
/*
	int key_mem = 7899;
	int key_semaphore = 9001;
*/
/*
	int key_mem = 65;
	int key_semaphore = 75;
*/
	int key_mem = 1000;
	int key_semaphore = 2000;
	int memid = shmget((key_t)key_mem, 10, 0777 | IPC_CREAT);
	if(memid == -1)
		perror("shmget = -1");

	errno = 0;
//	int semid = semget((key_t)key_semaphore, 1, 0);
	int semid = semget((key_t)key_semaphore, 1, 0777 | IPC_CREAT);
//	int semid = semget((key_t)key_semaphore, 0, 0);
	printf("errno after semget %i\n", errno);
	if(semid == -1)
		perror("semid = -1");
	printf("semid %i\n", semid);

	char* addr;
	errno = 0;
	addr = (char*)shmat(memid, 0, 0);
	if((void*)addr == -1 || errno != 0)
		perror("error shmat\n");

	int *pint;
	pint = (int*)addr;

	*pint = key_mem;
	//P(semid);

	int info = 0;


//	for(size_t i = 0; i < 1; ++i) {
		errno = 0;
		info = semctl(semid, 0, GETVAL);
		printf("errno %i\n", errno);
		printf("info %i\n", info);
		//if(info)


//	}


	shmctl(memid, IPC_RMID, 0);
	shmctl(semid, IPC_RMID, 0);

	return 0;
}
