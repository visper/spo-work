/*
Процесс 1 порождает потомка 2.
С помощью неименованого программного канала предать от потомка
родителю введенную с клавиатуры строку.
*/

#include <stdio.h> // output
#include <unistd.h> // fork, pipe
#include <string.h> // strlen
#include <sys/types.h> // exit
/*
//include for wait
#include <sys/types.h>
#include <sys/wait.h>
*/

#define LINE_LEN 32

int main(int argc, char** argv)
{
	int fd[2]; // descriptors for write and read
	           // fd[1] - writre
	           // fd[0] - read

	if(pipe(fd) == 1) {
		perror(argv[0]);
		exit(-1);
	}
/*
	printf("%i \t for write\n", fd[1]);
	printf("%i \t for read\n", fd[0]);
	printf("\n");
*/
	size_t pid1 = getpid();
	size_t pid2 = fork();

	if(pid2 == 0) { //child
		printf("process child\n");

//		char* line = 0; // for child read
		static char line[LINE_LEN];
		printf("enter line for send to chanel:\n");
		scanf("%s", line);
//		scanf("%a[a-z]", &line);

		if(line == 0) {
			perror("error read line");
			exit(-1);
		}

		printf("line after read from console: %s\n", line);

//		if(write(fd[1], line, strlen(line)+1) == -1)
		if(write(fd[1], line, LINE_LEN) == -1)
			perror("child write");
//		write(fd[1], line, LINE_LEN);

//		close(fd[1]);
//		close(fd[0]);
		printf("test\n");
	}

	else if(pid2 > 0) { //parent
		static char buf[LINE_LEN]; // for parent read
			while(1) {
			printf("test of parrent\n");
//			read(fd[0], buf, 8);
			if(read(fd[0], buf, LINE_LEN) == -1)
				perror("error read");
			if(buf != 0) { 
				printf("parent have: %s\n", buf);
	      			break;
			}
			}
		close(fd[0]);
		close(fd[1]);
		exit(0);
	}
	else { //error
		perror("some error");
		exit(-1);
	}
	return 0;
}
