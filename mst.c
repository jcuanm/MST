#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
	#define true 1
	#define false 0

struct node {
	int name;
	float weight;
	bool searched;
	struct node* next;
};

int length(struct node* head) {
	struct node* pointer = head;
	int counter = 0;

	while(pointer->next != NULL) {
		counter++;
		pointer = pointer->next;
	}

	return counter;
}
 
int main(int argc, char *argv[])
{

	char* flag = argv[1];
	int numpoints = atoi(argv[2]);
	char* numtrials = argv[3];
	char* dimensions = argv[4];

	// Creates a list of Node pointers of size numpoints
	struct node* vertices[numpoints];
	
	int i;
	
	for(i= 0; i < numpoints; i++) {
		
		struct node* vertex = malloc( sizeof(struct node) );
		vertex->name = i;
		vertex->weight = 0;
		vertex->searched = false;
		vertex -> next = NULL;
		
		//inserting the nodes into the array
		vertices[i] = vertex;
	}
	
	// Creates a linked list at each index
	// The version of C on Cloud9 doesn't support variable declarations inside of the for loop
	int k;
	int l;
	struct node* last_edge;
	
	for(k = 0; k < numpoints; k++) {

		printf("%i:", vertices[k] -> name);

		for (l = k + 1; l < numpoints; l++) {

			struct node* new_edge = malloc( sizeof(struct node) );
			
			new_edge -> name = l;
			new_edge -> weight = 2;					//TODO: CHANGE WEIGHT
			new_edge -> searched = false;
			new_edge -> next = NULL;

			printf("%i, ", new_edge -> name);
			
			if (l == (k + 1)) {
				vertices[k] -> next = new_edge;
				last_edge = new_edge;
			}
			else {
				last_edge -> next = new_edge;
				last_edge = new_edge;
			}
		}
		printf("\n");
	}

	printf("%i\n", length(vertices[0]));
	
	/*
	int j;
	
	for (j = 0; j < numpoints; j++){
		printf("name is: %f\n", vertices[j]->weight);
	}

 	printf("%s, %i, %s, %s\n", flag, numpoints, numtrials, dimensions);*/


	return 0;
}