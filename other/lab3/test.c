 #include <sys/types.h>
 #include <unistd.h>
 #define   MSGSIZE   20

 main(int argc, char **argv)
 {
     int fd[2]; pid_t pid;
     static char msgout[MSGSIZE]="Hello,world";
     static char msgin[MSGSIZE];

     if (pipe(fd) == -1) {
          perror(argv[0]);
            exit(1);
     }

     if ((pid=fork()) > 0) {  /* parent */
         write(fd[1], msgout, MSGSIZE);
     }
     else if (pid == 0) {      /* child */
          read(fd[0], msgin, MSGSIZE);
          puts(msgin);
	  exit(0);
     }
     else {          /* cannot fork */
          perror(argv[0]);
          exit(2);
     }

     exit(0);
 }
