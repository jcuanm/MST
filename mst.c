#include <stdio.h>

typedef int bool;
	#define true 1
	#define false 0

struct Node {
	char name[50];
	float weight;
	bool searched;
	struct Node* next;
};
 
int main(int argc, char *argv[])
{

	char* flag = argv[1];
	char* numpoints = argv[2];
	char* numtrials = argv[3];
	char* dimensions = argv[4];

 	printf("%s, %s, %s, %s\n", flag, numpoints, numtrials, dimensions);

  return 0;
}