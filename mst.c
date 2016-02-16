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
		vertex->weight = 2;
		vertex->searched = false;
		
		//inserting the nodes into the array
		vertices[i] = vertex;
	}
	
	// Creates a linked list at each index
	// The version of C on Cloud9 doesn't support variable declarations inside of the for loop
	int k;
	int l;
	
	for(k = 0; k<numpoints; k++){
		struct node* tail = malloc(sizeof(struct node));
		tail -> next = NULL;
		vertices[k] -> next = tail;

		for (l = k + 1; l<numpoints;l++){
			struct node* new_vertex = malloc(sizeof(struct node));
			new_vertex = vertices[l];
			// The commented line below is giving me issues. I'm trying to get new_vertex to point to the same thing that the node in the array is pointin to.
			//new_vertex -> next = vertices[k];
			vertices[k] -> next = new_vertex;
		}
	}
	
	
	int j;
	
	for (j = 0; j < numpoints; j++){
		printf("name is: %f\n", vertices[j]->weight);
	}


	for(i= 0; i < numpoints; i++) {
		
		struct node* vertex = malloc( sizeof(struct node) );
		vertex->name = i;
		vertex->weight = 2;
		vertex->searched = false;

		printf("name is: %f\n", vertex->weight);
	}

 	printf("%s, %i, %s, %s\n", flag, numpoints, numtrials, dimensions);


	return 0;
}