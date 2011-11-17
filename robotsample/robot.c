#include<stdio.h>
int main()
{
	char str[12];

	scanf("%s", str);
	printf("N\n");
    fflush(stdout);
	scanf("%s", str);
	printf("N\n");
    fflush(stdout);
// Return '$' so the framework will stop
//	scanf("%s", str);
//	printf("$\n");
//  fflush(stdout);
	
	return 0;
}