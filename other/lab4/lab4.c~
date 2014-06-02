/*
Процессы 1,2,3 параллельны и независимы.
С помощью именованых программных каналов
организовать передачу от П2 к П1 вторую половину символов, полученных от П3.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // mknod
#include <sys/types.h> // mknod
#include <sys/stat.h> // mknod
#include <fcntl.h> // mknod

#define MAX_LINE 64
#define HALF_MAX_LINE 32

#define S_IFIFO 010000

int main()
{
//char *name = ""
//	int check = mknod("NP", S_IFIFO | 0660, 0);
	int check = mknod("NP", S_IFIFO | 0660, 0);

	if(check != 0) {
		perror("mknod");
		return -1;
	}

	int pid1 = 0, pid2 = 0, pid3 = 0;
	pid1 = getpid();

	if((pid2 = fork()) == 0) { // child 2
		pid2 = getpid();

		if((pid3 = fork()) == 0) { //child 3
			pid3 = getpid();

			static char line[MAX_LINE];
			printf("write line\n");
			scanf("%s", line);
			char temp[HALF_MAX_LINE];

			for(size_t i = 0; i < HALF_MAX_LINE; ++i) {
				temp[i] = line[i];
			}
			//send to process 2
			int check = open("NP", O_WRONLY);
			if(check == -1) {
				perror("not open");
				return -1;
			}
			write(check, temp, HALF_MAX_LINE);
		}
/*
		else {
			perror("error fork child");
			exit(-1);
		}
*/

		char temp[HALF_MAX_LINE];

		//read from chanel 
		int check = open("NP", O_RDWR);
		if(check == -1) {
			perror("not open");
			return -1;
		}
		read(check, temp, HALF_MAX_LINE);
		printf("read line from pipe by process 2: %s", temp);

		//send to process 1
		write(check, temp, HALF_MAX_LINE);

	}
	else if (pid2 > 0) { // parent
		int check = open("NP", O_RDONLY);
		if(check == -1) {
			perror("not open");
			return -1;
		}

		char temp[HALF_MAX_LINE];
		read(check, temp, HALF_MAX_LINE);
		printf("read line from pipe by parent: %s\n", temp);

	}
	else {
		perror("error fork parrent");
		exit(-1);
	}

	unlink("NP");
	return 0;
}
