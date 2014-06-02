#include <stdio.h>

int main()
{
	struct num {
	  /*
                int num1 = 1;
                int num2 = 10;
                int num3 = 100;
                int num4 = 1000;
                int num5 = 10000;
                int num6 = 2;
                int num7 = 20;
                int num8 = 200;
                int num9 = 2000;
                int num10 = 20000;
	  */
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

	struct num n;

	printf("len of struct before %i\n", sizeof(n));

	n.num1 = 1;
        n.num2 = 10;
        n.num3 = 100;
        n.num4 = 1000;
        n.num5 = 10000;
        n.num6 = 2;
        n.num7 = 20;
        n.num8 = 200;
        n.num9 = 2000;
        n.num10 = 20000;

	printf("len of struct after %i\n", sizeof(n));








	return 0;
}
