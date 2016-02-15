#include <stdio.h>
 
int main(int argc, char *argv[])
{

	char* flag = argv[1];
	char* numpoints = argv[2];
	char* numtrials = argv[3];
	char* dimensions = argv[4];

 	printf("%s, %s, %s, %s\n", flag, numpoints, numtrials, dimensions);

  return 0;
}