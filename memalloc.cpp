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
void initLists(freeNode *&);
void addFreeNode(freeNode *&,int,int);
void addAllocNode(allocNode *&,int,int,int);
bool memAlloc(freeNode*&, allocNode*&,int,int);
bool checkFree(freeNode *&, int,int &);
void sortLists(freeNode*&, allocNode *&);


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
		//merge the freelist
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
                if(atmp->leaseExpiry > asmallest->leaseExpiry) asmallest = atmp;
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
	
	
	while(a != NULL){
		if(a->leaseExpiry >= 60){
			allocNode *tmp = a;
			a = a->next;
			delete tmp;
		}else a = a->next;
	
	}

}




// creating to pointers to alloc and freeNode to start the lists
freeNode *freeList;
allocNode *allocList;



int main(){
long clock = 0;
	
	initLists(freeList);
	printf("Done Initializing..\n");
	dumpLists(freeList,allocList);
	if(memAlloc(freeList,allocList, 200,55) == true) printf("Successful allocation..\n");	
	dumpLists(freeList,allocList);
	if(memAlloc(freeList,allocList, 150,60) == true) printf("Successful allocation..\n");	
	dumpLists(freeList,allocList);
	if(memAlloc(freeList,allocList, 50,70) == true) printf("Successful allocation..\n");	
	dumpLists(freeList,allocList);
	if(memAlloc(freeList,allocList, 600,52) == true) printf("Successful allocation..\n");	
	dumpLists(freeList,allocList);
	deallocate(freeList, allocList, 10);
	dumpLists(freeList,allocList);

	//if(memAlloc(freeList,allocList, 200,55) == true) printf("Successful allocation..\n");
	//:else printf("ALLOCATION FAILED!");	
	
	dumpLists(freeList,allocList);
	//repeat
	//requestMemory();
	// check for expired leases
	//until ( ++clock == TIME_LIMIT);
return 0;
}

