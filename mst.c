#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef int bool;
	#define true 1
	#define false 0

struct intFloatTuple {
  int intPart;
  float floatPart;
};

struct node {
	int name;
	float weight;
	bool searched;
	struct node* next;
};

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
  while(node != NULL)
  {
   printf("(%i -> %f -> %i)", node->name, node->weight, node->searched);
   node = node->next;
  }
}
 
/* Function to insert a node at the beginging of the linked list */
void push(struct node** head_ref, int new_weight, int name)
{
  /* allocate node */
  struct node* new_node = (struct node*) malloc(sizeof(struct node));
  
  /* put in the weight  */
  new_node->weight = new_weight;
  new_node->name = name;
  
  /* link the old list off the new node */
  new_node->next = (*head_ref);    
  
  /* move the head to point to the new node */
  (*head_ref) = new_node;
} 

void markSearched(struct node* graph[], struct node* vertex, float weight) {
  struct node* pointer = vertex;

  while(pointer != NULL) {
    if(pointer->weight == weight) {
      //Mark edge with weight in graph[vertex] searched
      pointer->searched = true;

      //Mark vertex in graph searched
      graph[pointer->name]->searched = true;
      break;
    }
    else {
      pointer = pointer->next;
    }
  }
}

struct intFloatTuple find(struct node* queue, struct node* graph[], int numpoints, int name) {

  struct node* pointer = graph[name];
  float result = 2.0;
  int edgeName = 7;

  while(pointer != NULL) {

    if(pointer->searched == false) {

      //AND if the vertex isn't searched
      if(graph[pointer->name]->searched == true) {
        pointer = pointer->next;
      }
      else {
        result = pointer->weight;
        edgeName = pointer->name;
        break;
      }
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
      //free(prevPointer);
    }
    else {
      while (curPointer != NULL) {
        if (curPointer -> name == name) {
          prevPointer -> next = curPointer -> next;
          //free(curPointer);

          //return edge that will never be used
          break;
        }
        else {
          prevPointer = curPointer;
          curPointer = curPointer -> next; 
        }
      }
    }
  }
  
  struct intFloatTuple returnValue;
  returnValue.intPart = edgeName;
  returnValue.floatPart = result;

  return returnValue;
}

float prim(struct node* graph[], int numpoints) {

  int newVertex;
  struct intFloatTuple result;
  float weight = 0.0;
  float leastWeightEdge = 2.0;

  struct node* firstVertex = (struct node*) malloc(sizeof(struct node));
  firstVertex->name = graph[0]->name;
  firstVertex->weight = graph[0]->weight;
  firstVertex->next = NULL;

  struct node* queue = firstVertex;
  struct node* pointer = queue;
  struct node* leastWeightVertex;
  int vertexName;

  for(int i = 0; i < (numpoints - 1); i++) {
    leastWeightEdge = 2.0;
    pointer = queue;

    while(pointer != NULL) {

      //Find next edge
      result = find(queue, graph, numpoints, pointer->name);

      if(result.floatPart < leastWeightEdge) {
        leastWeightVertex = graph[result.intPart];
        leastWeightEdge = result.floatPart;
        vertexName = pointer->name;
        pointer = pointer->next;
      }
      else {
        pointer = pointer->next;
      }
    }

    markSearched(graph, graph[vertexName], leastWeightEdge);

    push(&(queue), 0, leastWeightVertex->name);
    weight += leastWeightEdge;
  }

  return weight;
}
 
int main(int argc, char *argv[]) {

	char* flag = argv[1];
	int numpoints = atoi(argv[2]);
	int numtrials = atoi(argv[3]);
	char* dimensions = argv[4];

  //run algorithm numtrials times
  
  for(int trial = 1; trial <= numtrials; trial++) {
    //start timing function
    clock_t start = clock(), diff;

  	// Creates a list of node pointers of size numpoints
  	struct node* vertices[numpoints];
  	
    struct node* vertex = malloc( sizeof(struct node) );
    vertex->name = 0;
    vertex->weight = 0;
    vertex->searched = true;
    vertex -> next = NULL;
    
    vertices[0] = vertex;

  	for(int i = 1; i < numpoints; i++) {
  		
  		struct node* vertex = malloc( sizeof(struct node) );
  		vertex->name = i;
  		vertex->weight = 0;
  		vertex->searched = false;
  		vertex -> next = NULL;
  		
  		//inserting the nodes into the array
  		vertices[i] = vertex;
  	}
  	
  	// Creates a linked list at each index
  	struct node* last_edge;
  	
  	for(int k = 0; k < numpoints; k++) {

  		for (int l = k + 1; l < numpoints; l++) {

  			// Creates a new node
  			struct node* new_edge = malloc( sizeof(struct node) );
  			
  			new_edge -> name = l;
        //seed random number generator
        srand(time(NULL));
  			new_edge -> weight = (float) rand() / (RAND_MAX);					
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
  	
  	// Takes care of base cases
  	if (numpoints == 2){
  	  struct node* base_pointer = vertices[0] -> next;
  	  float final_result = base_pointer -> weight;
  	  printf("%f", final_result);
  	  return final_result;
  	}
  	if (numpoints < 2){
  	  printf("0");
  	  return 0;
  	}

    for(int o = 0; o < numpoints; o++) {
      struct node** pointer = &vertices[o];
      MergeSort(pointer);
    }

    float finalWeight = prim(vertices, numpoints);

    //end timing function
    diff = clock() - start;

    printf("Trial %i Final Weight: %f \nTime taken: %lu ms \n", trial, finalWeight, (diff * 1000000 / CLOCKS_PER_SEC));
  }

	return 0;
}