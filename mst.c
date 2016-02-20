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

int listLength(struct node* head) {
	struct node* pointer = head;
	int counter = 0;

	while(pointer->next != NULL) {
		counter++;
		pointer = pointer->next;
		printf("%i\n", pointer->name);
	}

	return counter;
}

struct node* SortedMerge(struct node* a, struct node* b);
void FrontBackSplit(struct node* source, struct node** frontRef, struct node** backRef);

/* sorts the linked list by changing next pointers (not weight) */
void MergeSort(struct node** headRef)
{
  struct node* head = *headRef;
  struct node* a;
  struct node* b;
 
  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL))
  {
    return;
  }
 
  /* Split head into 'a' and 'b' sublists */
  FrontBackSplit(head, &a, &b); 
 
  /* Recursively sort the sublists */
  MergeSort(&a);
  MergeSort(&b);
 
  /* answer = merge the two sorted lists together */
  *headRef = SortedMerge(a, b);
}
 
/* See http://geeksforgeeks.org/?p=3622 for details of this 
   function */
struct node* SortedMerge(struct node* a, struct node* b)
{
  struct node* result = NULL;
 
  /* Base cases */
  if (a == NULL)
     return(b);
  else if (b==NULL)
     return(a);
 
  /* Pick either a or b, and recur */
  if (a->weight <= b->weight)
  {
     result = a;
     result->next = SortedMerge(a->next, b);
  }
  else
  {
     result = b;
     result->next = SortedMerge(a, b->next);
  }
  return(result);
}
 
/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
     and return the two lists using the reference parameters.
     If the length is odd, the extra node should go in the front list.
     Uses the fast/slow pointer strategy.  */
void FrontBackSplit(struct node* source,
          struct node** frontRef, struct node** backRef)
{
  struct node* fast;
  struct node* slow;
  if (source==NULL || source->next==NULL)
  {
    /* length < 2 cases */
    *frontRef = source;
    *backRef = NULL;
  }
  else
  {
    slow = source;
    fast = source->next;
 
    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL)
    {
      fast = fast->next;
      if (fast != NULL)
      {
        slow = slow->next;
        fast = fast->next;
      }
    }
 
    /* 'slow' is before the midpoint in the list, so split it in two
      at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
  }
}
 
/* Function to print nodes in a given linked list */
void printList(struct node *node)
{
  while(node!=NULL)
  {
   printf("(%i -> %f -> %i)", node->name, node->weight, node->searched);
   node = node->next;
  }
}

void enqueue(struct node* queue, struct node* vertex) {
  vertex -> next = queue -> next;
  queue -> next = vertex;
}
 
/* Function to insert a node at the beginging of the linked list */
void push(struct node** head_ref, int new_weight, int name)
{
  /* allocate node */
  struct node* new_node =
            (struct node*) malloc(sizeof(struct node));
  
  /* put in the weight  */
  new_node->weight  = new_weight;
  new_node->name = name;
  
  /* link the old list off the new node */
  new_node->next = (*head_ref);    
  
  /* move the head to point to the new node */
  (*head_ref)    = new_node;
} 

void markSearched(struct node* vertex, float weight) {
  struct node* pointer = vertex;

  while(pointer != NULL) {
    if(pointer->weight == weight) {

      pointer->searched = true;
      break;
    }
    else {
      pointer = pointer->next;
    }
  }
}

float find(struct node* queue, struct node* graph[], int numpoints, int name) {

  struct node* pointer = graph[name];
  float result = 2.0;

  while(pointer->next != NULL) {

    if(pointer->searched == false) {
      result = pointer->weight;
      break;
    }
    else {
      pointer = pointer->next;
    }
  }

  //DELETION of a node from the queue
  if(result == 2.0) {
    struct node* curPointer = queue->next;
    struct node* prevPointer = queue;

    if(prevPointer -> name == name) {
      queue = queue -> next;
      free(prevPointer);
    }
    else {
      while (curPointer != NULL) {
      
        if (curPointer -> name == name) {
          prevPointer -> next = curPointer -> next;
          free(curPointer);
        }
        else {
          prevPointer = curPointer;
          curPointer = curPointer -> next; 
        }
      }
    }
  }
  return result;
}

float prim(struct node* graph[], int numpoints) {

  int newVertex;
  float result;
  float weight = 0.0;
  float leastWeightEdge = 2.0;

  struct node* firstVertex = (struct node*) malloc(sizeof(struct node));
  firstVertex->name = graph[0]->name;
  firstVertex->weight = graph[0]->weight;
  firstVertex->next = NULL;

  struct node* queue = firstVertex;
  struct node* pointer = queue;
  struct node* leastWeightVertex;

  while(pointer != NULL) {

    //Find next edge
    result = find(queue, graph, numpoints, pointer->name);

    if(result < leastWeightEdge) {
      leastWeightVertex = graph[pointer->name];
      leastWeightEdge = result;
      pointer = pointer->next;

    }
    else {
      pointer = pointer->next;
    }
  }

  struct node* newQVertex = (struct node*) malloc(sizeof(struct node));
  newQVertex->name = leastWeightVertex->name;
  newQVertex->weight = 0.0;
  newQVertex->next = NULL;

  markSearched(leastWeightVertex, leastWeightEdge);
  enqueue(queue, newQVertex);
  weight += leastWeightEdge;
  printf("%f\n", leastWeightEdge);

  return weight;
}
 
int main(int argc, char *argv[]) {

	char* flag = argv[1];
	int numpoints = atoi(argv[2]);
	char* numtrials = argv[3];
	char* dimensions = argv[4];

	// Creates a list of node pointers of size numpoints
	struct node* vertices[numpoints];

	int i;
	
	for(i= 0; i < numpoints; i++) {
		
		struct node* vertex = malloc( sizeof(struct node) );
		vertex->name = i;
		vertex->weight = 0;
		vertex->searched = true;
		vertex -> next = NULL;
		
		//inserting the nodes into the array
		vertices[i] = vertex;
	}
	
	// Creates a linked list at each index
	// This version of C on Cloud9 doesn't support variable declarations inside of the for loop

	int k;
	int l;
	struct node* last_edge;
	
	for(k = 0; k < numpoints; k++) {

		for (l = k + 1; l < numpoints; l++) {

			// Creates a new node
			struct node* new_edge = malloc( sizeof(struct node) );
			
			new_edge -> name = l;
			new_edge -> weight = ((double) rand() / (RAND_MAX));					
			new_edge -> searched = false;
			new_edge -> next = NULL;
			
			// Process of adding the new node to the appropriate position in the linked list
			if (l == (k + 1)) {
				vertices[k] -> next = new_edge;
				last_edge = new_edge;
			}
			else {
				last_edge -> next = new_edge;
				last_edge = new_edge;
			}
		}
	}

  for(int k=0; k < numpoints; k++) {
    struct node** pointer = &vertices[k];
    MergeSort(pointer);
    printList(vertices[k]);
    printf("\n");
  }
	
  printf("Final Weight: %f \n", prim(vertices, numpoints));

	return 0;
}