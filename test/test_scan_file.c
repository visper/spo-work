#include <stdio.h>
#include <stdlib.h>

FILE *file2;


int main()
{
	file2 = fopen("file2", "r");
	if(!file2) {
		printf("error input file2!\n");
		return -1;
	}
	int tmp;
	fscanf(file2, "%i", &tmp);

	printf("TEST\n");

	printf("test %i\n", tmp);
//	*shm = tmp;
//	fscanf(file2, "%i", &(*ptr->A));			
	fclose(file2);//close file

	return 0;
}
