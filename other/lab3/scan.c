#include <stdio.h>

int main()
{
//	char* line;
	char line[25];

	printf("write\n");
//	scanf("%a[a-z]", &line);
	scanf("%s", line);

	printf("read THIS: %s\n", line);

	return 0;
}
