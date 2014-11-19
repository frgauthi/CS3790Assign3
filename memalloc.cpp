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
	struct range *hole;
	struct freeNode *next;

};


// allocnode contains range structure, an int for lease time, and a pointer to the next allocNode
struct allocNode{
	struct range *allocated;
	int leaseExpiry;
	struct allocNode *next;
};




// function prototypes
void dumpLists(freeNode*,allocNode*);
freeNode *initLists(freeNode*);



// Function bodies
void dumpLists(freeNode *free, allocNode *alloc){
	
	//dump freelist
	freeNode *tmpf = free;
	printf("\nFreeNodes:\n");
	while(tmpf != NULL){
		printf("node: Start = %d , Size = %d  \n",tmpf->hole->start,tmpf->hole->size);
		tmpf = tmpf->next;	
	}

	//dump allocList
	allocNode *tmpa = alloc;
	printf("\nAllocNodes:\n");
	while(tmpa != NULL){
		printf("node: Start = %d , Size = %d  \n",tmpa->allocated->start,tmpa->allocated->size);	
		tmpa = tmpa->next;
	}
	
}

freeNode *initLists(){
	freeNode *tmp = new freeNode;
	range *tmpRange = new range;
	tmpRange->size = MEMORY_SIZE;
	tmpRange->start = 0;
	tmp->hole = tmpRange;
	return tmp;
	

}

freeNode *remRange(freeNode free){

}


//returns a pointer to an allocated node
// simulates a memory request by asking for a randomized size of node
// takes the allocated and freelist references as a parameter as well as the range and start reference to be modified 
//allocNode *requestMemory(allocNode &alloc,freeNode &free,int range,int &start);
//{
	
	
	// check freelist for available space
	// remove the range indicated from freelist
	// add the range to a new node in alloclist
	 

//}


// creating to pointers to alloc and freeNode to start the lists
freeNode *freeList;
allocNode *allocList;



int main(){
long clock = 0;
	
	freeList = initLists();
	printf("Done Initializing..");
	dumpLists(freeList,allocList);
	//repeat
	//requestMemory();
	// check for expired leases
	//until ( ++clock == TIME_LIMIT);
return 0;
}

