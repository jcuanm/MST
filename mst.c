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
	struct Node* next;
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
		
		vertices[i] = vertex;
	}
	
	// Creates a linked list at each index
	int k;
	int l;
	
	for(k = 0; k<numpoints; k++){
		vertices[k] -> next = NULL;
		struct node* head = malloc(sizeof(struct node));
		struct node* head -> next = NULL;
		

		/*
		for (l = k + 1; l<numpoints;l++){
			new_vertex = vertices[l]; 
			
		}*/
	}
	
	
	int j;
	
	for (j = 0; j < numpoints; j++){
		printf("name is: %f\n", vertices[j]->weight);
	}

	return 0;
}