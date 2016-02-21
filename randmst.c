#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//Define Boolean type
typedef int bool;
	#define true 1
	#define false 0

//Special data structure specifically for our find function
struct intFloatTuple {
  int intPart;
  float floatPart;
};

//Struct to handle coordinates in up to four dimensions
struct coordTuple {
  float x;
  float y;
  float z;
  float w;
};

//Nodes for linked lists
struct node {
	int name;
	float weight;
	bool searched;
	struct node* next;
};

//type templates for our Mergesort helper functions
struct node* SortedMerge(struct node* a, struct node* b);

void FrontBackSplit(struct node* source, struct node** frontRef, struct node** backRef);

// In order to optimize our algorithm, we use MergeSort on linked lists to 
// ensure that the minimum weight edges from any vertex will simply be the 
// first valid node that we encounter

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
 
//Sorted merge takes two sorted linked lists a and b and 
//turns them into a single sorted list

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
 
//Split the nodes of the given list into front and back halves,
//and return the two lists using the reference parameters.
//If the length is odd, the extra node should go in the front list.

void FrontBackSplit(struct node* source, struct node** frontRef, struct node** backRef) {
  
  struct node* fast;
  struct node* slow;

  if (source == NULL || source->next == NULL) {
    *frontRef = source;
    *backRef = NULL;
  }
  else {
    slow = source;
    fast = source->next;

    while (fast != NULL) {
      fast = fast->next;

      if (fast != NULL) {
        slow = slow->next;
        fast = fast->next;
      }
    }
 
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
  }
}
 
// Function to print nodes in a given linked list
// Used to test correctness of our MST algorithm

void printList(struct node *node)
{
  while(node != NULL)
  {
   printf("(%i -> %f -> %i)", node->name, node->weight, node->searched);
   node = node->next;
  }
}
 
// Function to insert a node at the beginging of the linked list

void push(struct node** head_ref, int new_weight, int name) {
  struct node* new_node = (struct node*) malloc(sizeof(struct node));
  
  new_node->weight = new_weight;
  new_node->name = name;
  new_node->next = (*head_ref);    
  
  *head_ref = new_node;
} 

//Takes in an edge weight and the head vertex, and marks the correct edge as searched

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

//Calculates distances given two coordinate tuples in dimensions 0, 2, 3, or 4

float distance(int dimensions, struct coordTuple originCoord, struct coordTuple destCoord) {
  if(dimensions == 2) {
    return sqrt(pow(originCoord.x - destCoord.x, 2) + pow(originCoord.y - destCoord.y, 2));
  }
  else if(dimensions == 3) {
    return sqrt(pow(originCoord.x - destCoord.x, 2) + pow(originCoord.y - destCoord.y, 2) 
      + pow(originCoord.z - destCoord.z, 2));
  }
  else if(dimensions == 4) {
    return sqrt(pow(originCoord.x - destCoord.x, 2) + pow(originCoord.y - destCoord.y, 2) 
      + pow(originCoord.z - destCoord.z, 2) + pow(originCoord.w - destCoord.w, 2));
  }
  else {
    return 0.0;
  }
}

// Given a vertex, find will return the first edge that has not been searched and does not lead
// to another vertex that has already been searched. If all the edges out of the vertex have been
// searched or lead to searched vertices, find() removes the vertex from the queue and returns an 
// edge weight = 10.0 that is guaranteed not to be chosen as the global least weight edge 
// (since the valid edge weights only lie between 0 and 1)

struct intFloatTuple find(struct node* queue, struct node* graph[], int numpoints, int name) {

  struct node* pointer = graph[name];

  //Use these initial values as defaults
  float result = 10.0;
  int edgeName = 10;

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
    }
    else {
      while (curPointer != NULL) {
        if (curPointer -> name == name) {
          prevPointer -> next = curPointer -> next;

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

// prim() runs Prim's algorithm for finding MST given a graph
// and the number of vertices. It accomplishes this with the use of helper
// functions including find, markSearched, and push that have been previously
// defined above.

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

// In main, we accomplish a number of things including:
// 1. Create an adjacency list representation of a complete graph 
// 2. Runs Prims algorithm on the graph and prints the weight and time taken
// 3. Repeats the above numtrials number of times
// 4. Averages the times and weights of the trials
// The result is that we print the time and weight for each trial as well
// as the average for all trials
 
int main(int argc, char *argv[]) {

	int flag = atoi(argv[1]);
	int numpoints = atoi(argv[2]);
	int numtrials = atoi(argv[3]);
	int dimensions = atoi(argv[4]);

  float averageWeight = 0.0;
  float averageTime = 0.0;
  srand(time(NULL));

  //run algorithm numtrials times
  for(int trial = 1; trial <= numtrials; trial++) {
    
    //start timing function
    clock_t start = clock(), diff;

    //create array of coordinates
    struct coordTuple array[numpoints];

    // Create coordinate tuples depending on the number
    // of dimensions we're considering 

    if(dimensions == 0) {
      // we'll take care of this in distance function by
      // simply returning a random edge (no need for coordinates)
    }
    else if(dimensions == 2) {

      for(int i = 0; i < numpoints; i++) {
        struct coordTuple coord;
        coord.x = (float) rand() / (RAND_MAX);
        coord.y = (float) rand() / (RAND_MAX);
        array[i] = coord;
      }
    }
    else if(dimensions == 3) {

      for(int i = 0; i < numpoints; i++) {
        struct coordTuple coord;
        coord.x = (float) rand() / (RAND_MAX);
        coord.y = (float) rand() / (RAND_MAX);
        coord.z = (float) rand() / (RAND_MAX);
        array[i] = coord;
      }
    }
    else if(dimensions == 4) {

      for(int i = 0; i < numpoints; i++) {
        struct coordTuple coord;
        coord.x = (float) rand() / (RAND_MAX);
        coord.y = (float) rand() / (RAND_MAX);
        coord.z = (float) rand() / (RAND_MAX);
        coord.w = (float) rand() / (RAND_MAX);
        array[i] = coord;
      }
    }
    else {
      printf("Not valid dimension\n");
      return 0;
    }

  	// Creates a list of node pointers of size numpoints
    // each pointer in vertices[i] points to a list of all edges 
    // from vertex i to all other vertices

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
  		
  		// Insert the nodes into the array
  		vertices[i] = vertex;
  	}
  	
  	// Creates a linked list at each index
  	struct node* last_edge;
  	
    // For each vertex in the graph...

  	for(int k = 0; k < numpoints; k++) {
      struct coordTuple originCoord = array[k];

      // For each edge in the graph...

  		for (int l = k + 1; l < numpoints; l++) {

        struct coordTuple destCoord = array[l];

  			// Creates a new node
  			struct node* new_edge = malloc( sizeof(struct node) );
  			
  			new_edge -> name = l;

        if(dimensions != 0) {
  			 new_edge -> weight = distance(dimensions, originCoord, destCoord);			
        }
        else {
          new_edge -> weight = (float) rand() / RAND_MAX;
        }

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
  	
  	// Take care of base cases
  	if (numpoints == 2){

  	  struct node* base_pointer = vertices[0] -> next;
  	  float final_result = base_pointer -> weight;
  	  printf("%f", final_result);

  	  return final_result;
  	}
    else if (numpoints < 2){
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

    printf("Trial %i Final Weight: %f \nTime taken: %lu ms \n", trial, finalWeight, (diff * 1000 / CLOCKS_PER_SEC));
    
    averageTime += (diff * 1000 / CLOCKS_PER_SEC);
    averageWeight += finalWeight;
  }

  printf("Average Time: %f, Average Weight: %f\n", averageTime / numtrials, averageWeight / numtrials);

	return 0;
}