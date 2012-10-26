#include <stdio.h> 

int main(int argc, char* argv[]) 
{ 
	int n = 5, i = 0;

	for(i = 0; i < 3; i++)
	{
		printf("[stdout] - Please input a integer: \n");
		scanf("%d", &n);
		
		fprintf(stderr, "[stderr] - You input the number: %d\n", n);
	}

	return 0;
} 
