#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

#define MIN_LEASE 40
#define MAX_LEASE 70
#define MIN_SIZE  50
#define MAX_SIZE  350
#define TIME_LIMIT 1000
#define MIN_LEASE 40
#define MEMORY_SIZE 1000

// statistics variables
int totalRequests, satisfied, denied;
int smallestBlock, largestBlock, sumBlock;
int shortestLease, longestLease, sumLease;
int mergeCount;



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
void initLists(freeNode *&);
void addFreeNode(freeNode *&,int,int);
void addAllocNode(allocNode *&,int,int,int);
bool memAlloc(freeNode*&, allocNode*&,int,int);
bool checkFree(freeNode *&, int,int &);
void sortLists(freeNode*&, allocNode *&);
void freeMerge(freeNode*&);


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

void initLists(freeNode *&f){

	// create initial freeNode
	freeNode *tmp = new freeNode;
	
	// assign range and return
	tmp->hole.size = MEMORY_SIZE;
	tmp->hole.start = 0;
	tmp->next = f;
	f = tmp;
}


void addAllocNode(allocNode *&f,int start, int size, int lease){
	//create node
	allocNode *tmp = new allocNode;
	
	//assign range
	tmp->allocated.start = start;
	tmp->allocated.size = size;
	tmp->leaseExpiry = lease;
	tmp->next = f;
	f = tmp;

	
}


void addFreeNode(freeNode *&f, int start, int size){
	//create node
	freeNode *tmp = new freeNode;
	
	//assign range to the node
	tmp->hole.size = size;
	tmp->hole.start = start;
	tmp->next = f;
	f = tmp;
	
}

bool memAlloc(freeNode *&free, allocNode *&alloc, int size, int lease){
	int start;
	
	
	if(checkFree(free,size,start)){ 
		addAllocNode(alloc,start,size,lease);
		//add to succeeded allocs
		sortLists(free,alloc);
		return true;
		
	}else{
		// merge and sort
		freeMerge(free);
		mergeCount++;
		sortLists(free,alloc);

		//try allocation again
		if(checkFree(free,size,start)){
			addAllocNode(alloc,start,size,lease);
			//add to succeeded allocs
			sortLists(free,alloc);
			return true;
		}
	}
// add to failed allocs
return false;
}


//checks for and removes a chunk of free space
// returns the start location for the free space;
bool checkFree(freeNode *&f, int size, int &start){
 	freeNode *tmp = f;
	

	while(tmp != NULL){
		if(tmp->hole.size >= size ) {
			start = tmp->hole.start;
			tmp->hole.start += size;
			tmp->hole.size -= size;
			return true;
		}
		tmp = tmp->next;
				
	}
return false;
}


void sortLists(freeNode *&f, allocNode*&a){
	freeNode *tmp = f;
	freeNode *smallest = tmp;
	freeNode *anchor = f;
	int tmpstart;
	int tmpsize;
	
	
     while(anchor !=NULL){
	//find smallest
	tmp = anchor;
	smallest = anchor;
	while(tmp != NULL){
		if(tmp->hole.start < smallest->hole.start) smallest = tmp;
		tmp = tmp->next;
	}
	
	//swap
	tmpstart = anchor->hole.start;
	tmpsize = anchor->hole.size;
	anchor->hole.start = smallest->hole.start;
	anchor->hole.size = smallest->hole.size;
	smallest->hole.start = tmpstart;
	smallest->hole.size = tmpsize;

	//advance anchor
	anchor = anchor->next;
    }
	///// SORT ALLOCLIST /////
	allocNode *atmp = a;
        allocNode *asmallest = atmp;
        allocNode *aanchor = a; 
	int tmplease;

     while(aanchor !=NULL){
        //find smallest
	atmp = aanchor;
	asmallest = aanchor;
        while(atmp != NULL){
		if(atmp->leaseExpiry < asmallest->leaseExpiry) asmallest = atmp;
                atmp = atmp->next;
        }

        //swap
        tmpstart = aanchor->allocated.start;
        tmpsize = aanchor->allocated.size;
	tmplease = aanchor->leaseExpiry;
        aanchor->allocated.start = asmallest->allocated.start;
        aanchor->allocated.size = asmallest->allocated.size;
	aanchor->leaseExpiry =  asmallest->leaseExpiry;
        asmallest->allocated.start = tmpstart;
        asmallest->allocated.size = tmpsize;
	asmallest->leaseExpiry = tmplease;

        //advance anchor
        aanchor = aanchor->next;
    }



}

void deallocate(freeNode *&f, allocNode *&a, long clock){
	
	if(a != NULL){
		while((a != NULL) && (a->leaseExpiry <= clock)){
			allocNode *tmp = a;
			addFreeNode(f,tmp->allocated.start, tmp->allocated.size);
			a = a->next;
			delete tmp;
		}
	}
	sortLists(f,a);
}

void freeMerge(freeNode *&f){
	if(f != NULL){
		freeNode *Left = f;
		freeNode *Right;
		while(Left->next !=NULL){
			Right = Left->next;
			if(Left->hole.size + Left->hole.start == Right->hole.start ){
				Left->hole.size += Right->hole.size;
				Left->next = Right->next;
				delete Right;
				freeNode *Right;
			}else Left = Left->next;
		
		}
	}
	
}

// creating to pointers to alloc and freeNode to start the lists
freeNode *freeList;
allocNode *allocList;



int main(){
int clock = 0;
int size = 0;
int lease = 0;

	srand(time(NULL));
	initLists(freeList);
	smallestBlock = MAX_SIZE;
	largestBlock = MIN_SIZE;
	shortestLease = MAX_LEASE;
	longestLease = MIN_LEASE;	

	do{
	
		deallocate(freeList,allocList, clock);
		
		//attempt allocation
		if(clock % 20 == 0){
			//calculate random lease and size
			lease = clock + (rand() % (MAX_LEASE - MIN_LEASE) + MIN_LEASE);
			size = rand() % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
			
			//calculate values for statistics
			if(lease > longestLease) longestLease = lease;
			if(lease < shortestLease) shortestLease = lease;
			if(size < smallestBlock) smallestBlock = size;
			if(size > largestBlock) largestBlock = size;
			
		
			if(memAlloc(freeList,allocList, size,lease) == true) { 
						sumLease+=lease; sumBlock+=size; satisfied++; totalRequests++; 
			}
			else { denied++; totalRequests++; }	
		
		}
	
	}while( ++clock != TIME_LIMIT);
	
	printf(" Total Requests: %5d\t Satisfied Requests: %5d\t Denied Requests: %5d\n Smallest Block: %5d\t Largest Block: %5d\t\t Avg Block Size: %5d\n Shortest Lease: %5d   Longest Lease: %5d \t\t Avg Lease Size: %5d\n\n",totalRequests, satisfied, denied, smallestBlock, largestBlock, (sumBlock/totalRequests),
		shortestLease, longestLease, (sumLease/totalRequests));	

	dumpLists(freeList,allocList);

return 0;
}

