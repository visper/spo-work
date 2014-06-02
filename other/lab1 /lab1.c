/*
  Написать программу, выводящую в файл протокола список файлов из указанного
  каталога, имеющих n и более ссылок. Количество ссылок n, а также имена файла
  протокола и каталога вводятся в командной строке.
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
//for dir
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>

#define S_IFMT  0170000
#define S_IFDIR 0040000

//argv[1] - dir for find files with refences
//argv[2] - file for output information
int main(int argc, char** argv)
{
        //check arguments
	if(argc != 3) {
		printf("error of input information");
		return -1;
	}

        //file create or if it already create open
        //and delete all information inside
        //int file_out = open(argv[2], O_WRONLY);
        //int file_out = open(argv[2], O_CREAT | O_RDWR);
	FILE *file_out;
	errno = 0;
	file_out = fopen(argv[2], "w+");
	if(errno == -1) {
		printf("error file for out\n");
		return -1;
	}


        //must to read inf about dir and check all files in loop
	DIR *dir;
	struct dirent *entry;

	dir = opendir(argv[1]);
	if(!dir) {
		perror("diropen");
		return -1;
	}

	struct stat stbuf;

	while( (entry = readdir(dir)) != NULL) {
        /*
                printf("%d - %s [%d] %d\n",
		       entry -> d_ino, entry -> d_name, entry -> d_type,
		       entry-> d_reclen);
        */
        //сюда втюхать проверку и запись в файл
		
	//inf about file
	//stat(entry->d_ino, &stbuf);
	  size_t len_name = strlen(entry->d_name);
	  len_name += 1;
	  size_t len_way = strlen(argv[1]);

	  printf("%zu len_name\n", len_name);
	  printf("%zu len_way\n", len_way);

	  //_____________________commit
	  /*
	  char* result = strncat(,argv[1], len_way);
	  
	  //	  char* res = strcat(argv[1], entry->d_name);
	  char* result = strcat(result, entry->d_name);
	  */
	  //	  char* res = strcat(argv[1], entry->d_name);
	  /*
	  char* new_name = (char*)malloc(len_name*sizeof(char));
	  for(int i = 0; i < len_name; ++i) {
		  //new_name[i] = argv[1][i];
		  new_name[i] = entry->d_name[i];
	  }
	  new_name[len_name-1] = '\0';
//	  char* res = strncat(argv[1], new_name, (len_name - 4));
	  char* res = strcat(argv[1], new_name);
//	  char* res = strncat(argv[1], entry->d_name, len_name);
	  printf("%s res\n", res);
	  printf("**************\n");
	  for(int i = 0; i < len_name; ++i) {
		  free(new_name[i]);
	  }
	  free(new_name);
	  */
	  //_____________________commit

	  /*
		//create a string with way and name of file
		int len_name = 0;
		len_name = sizeof(entry->d_name)/sizeof(char);
		printf("%i", len_name);
		//как соединить имя папки и имя файла???
		//strcat
		int len_way = 0;
		len_way = sizeof(argv[1])/sizeof(char);
		printf("%i", len_way);
	  */

		stat(entry->d_name, &stbuf);//Epic Fail
		if(stbuf.st_nlink > 1) {
			fprintf(file_out, "%s\n", entry->d_name);//SIGSEGV
		}

	}
	closedir(dir);

        /*
	printf("st_dev %i\n", stbuf.st_dev);
	printf("st_ino %i\n", stbuf.st_ino);
	printf("st_mode %i\n", stbuf.st_mode);
	printf("st_nlink %i\n", stbuf.st_nlink);
	printf("st_uid %i\n", stbuf.st_uid);
	printf("st_gid %i\n", stbuf.st_gid);
	printf("st_rdev %i\n", stbuf.st_rdev);
	printf("st_size %i\n", stbuf.st_size);
	printf("st_blksize %i\n", stbuf.st_blksize);
	printf("st_blocks %i\n", stbuf.st_blocks);
	printf("st_atime %i\n", stbuf.st_atime);
	printf("st_mtime %i\n", stbuf.st_mtime);
	printf("st_ctime %i\n", stbuf.st_ctime);
        */

	return 0;
}
