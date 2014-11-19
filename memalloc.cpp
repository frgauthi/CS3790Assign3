#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define MIN_LEASE 40
#define MAX_LEASE 70
#define MIN_SIZE  50
#define MAX_SIZE  350
#define TIME_LIMIT 1000
#define MIN_LEASE 40
#define MEMORY_SIZE 1000



// range holes two ints, a start location and a size of memory
struct range {
	int start;		// the starting address of the range
	int size;		// size of the range
};


// freeNode contains a range structure and a pointer to the next freeNode in the list
struct freeNode{
	struct range hole;
	struct freeNode *next;

};


// allocnode contains range structure, an int for lease time, and a pointer to the next allocNode
struct allocNode{
	struct range allocated;
	int leaseExpiry;
	struct allocNode *next;
};




// function prototypes
void dumpLists(freeNode*,allocNode*);
freeNode *initLists(freeNode*);
freeNode *addFreeNode(freeNode *);
allocNode *addAllocNode(allocNode*,int,int,int);


// Function bodies
void dumpLists(freeNode *free, allocNode *alloc){
	
	//dump freelist
	freeNode *tmpf = free;
	printf("\nFreeNodes:\n");
	while(tmpf != NULL){
		printf("node: Start = %d , Size = %d  \n",tmpf->hole.start,tmpf->hole.size);
		tmpf = tmpf->next;	
	}

	//dump allocList
	allocNode *tmpa = alloc;
	printf("\nAllocNodes:\n");
	while(tmpa != NULL){
		printf("node: Start = %d , Size = %d, Expiry = %d  \n",tmpa->allocated.start,tmpa->allocated.size,
												tmpa->leaseExpiry);	
		tmpa = tmpa->next;
	}
	
}

freeNode *initLists(){

	// create initial freeNode
	freeNode *tmp = new freeNode;
	
	// assign range and return
	tmp->hole.size = MEMORY_SIZE;
	tmp->hole.start = 0;
	return tmp;
}


allocNode *addAllocNode(allocNode *f,int start, int size, int lease){
	//create node
	allocNode *tmp = new allocNode;
	
	//assign range
	tmp->allocated.start = start;
	tmp->allocated.size = size;
	tmp->leaseExpiry = MAX_LEASE;
	tmp->next = f;
	

	return tmp;
}







// creating to pointers to alloc and freeNode to start the lists
freeNode *freeList;
allocNode *allocList;



int main(){
long clock = 0;
	
	freeList = initLists();
	printf("Done Initializing..");
	allocList = addAllocNode(allocList,0,200,55);
	dumpLists(freeList,allocList);
	//repeat
	//requestMemory();
	// check for expired leases
	//until ( ++clock == TIME_LIMIT);
return 0;
}

