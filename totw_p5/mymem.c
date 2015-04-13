#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>


/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList
{
  // doubly-linked list
  struct memoryList *prev;
  struct memoryList *next;

  int size;            // How many bytes in this block?
  char alloc;          // 1 if this block is allocated,
                       // 0 if this block is free.
  void *ptr;           // location of block in memory pool.
};
 
strategies myStrategy = NotSet;    // Current strategy


size_t mySize;
void *myMemory = NULL;

static struct memoryList *head;

/* for next-fit, this points to the next block */
/* for others, it points to the first unused block*/
static struct memoryList *next;

/* For releasing memory in initmem */
void release() {
  while(head != NULL) {
    struct memoryList *tmp = head;
    head = head->next;
    free(tmp);
  }
}

/* For initializing memory initmem */
void initialize() {
  head = malloc(sizeof(struct memoryList));
  head->prev = head->next = NULL;
  head->alloc = 0;
  head->size = mySize;
  head->ptr = myMemory;

  next = head;
}

/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void initmem(strategies strategy, size_t sz) {
  myStrategy = strategy;
  
  /* all implementations will need an actual block of memory to use */
  mySize = sz;

  if (myMemory != NULL)
    free(myMemory); /* in case this is not the first time initmem2 is called */

  /* TODO: release any other memory you were using for bookkeeping when doing a re-initialization! */
  release();

  myMemory = malloc(sz);
	
  /* TODO: Initialize memory management structure. */
  initialize();
}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */

void *mymalloc(size_t requested) {
  assert((int)myStrategy > 0);

  struct memoryList *to_use = NULL; /*Which chunk of memory are we going to use?*/
  struct memoryList *tmp = NULL; /* Some will use this */
	
  switch (myStrategy)
  {
  case NotSet: 
    return NULL;
  case First:
    if(head != NULL) {
      to_use = head;
      while(to_use != NULL) {
	if(to_use->size >= requested && !to_use->alloc) {
	  break;
	} else {
	  to_use = head->next;
	}
      }
    }
    break;
  case Best:
    tmp = head;
    while(tmp != NULL) {
      if((!tmp->alloc && tmp->size >= requested) && (!to_use || tmp->size > to_use->size)) {
	to_use = tmp;
      }
      tmp = tmp->next;
    }
    break;
  case Worst:
    tmp = head;
    while(tmp != NULL) {
      if((!tmp->alloc && tmp->size >= requested) && (!to_use || tmp->size < to_use->size)) {
	to_use = tmp;
      }
      tmp = tmp->next;
    }
    break;
  case Next:
    tmp = next;
    
    while(tmp != NULL) {
      if(tmp->size >= requested && !tmp->alloc) {
	next = to_use = tmp;
	break;
      }
      
      tmp = tmp->next;
      if(!tmp)
	tmp = head;
      if(tmp == next)
	break;
    }
    break;
  }

  if(to_use) {
    if(to_use->size == requested) {
      /* simple case, no splitting needed */
      to_use->alloc = 1;
      return to_use;
    } else {
      /* allocate what's needed, but create a new entry for the rest */
      int extra = to_use->size - requested;
      struct memoryList *new_list = malloc(sizeof(struct memoryList));
      new_list->next = to_use->next;
      new_list->next->prev = new_list;
      new_list->prev = to_use;
      to_use->next = new_list;
      
      new_list->alloc = 0;
      to_use->alloc = 1;
      new_list->size = extra;
      to_use->size = requested;
      new_list->ptr = to_use->ptr + requested;
    }
  }
  return to_use;
}


/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block) {
  struct memoryList *tmp = NULL;
  
  if(head != NULL){
    tmp = head;
    /*Go through memory to find the thing to free*/
    while(tmp->next != NULL){
      if(tmp->ptr == block){
	tmp->alloc = '0';
      }
      tmp = tmp->next;
    }
  }
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when we refer to "memory" here, we mean the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes() {
  int holes = 0;
  struct memoryList *tmp = NULL;
  /* Make sure head isn't null before we go through and accidentally segfault */
  if(head != NULL) {

    /* Gotta start at the head */
    if(head->alloc == 0){
      holes++;
    }

    /* Go through the memory */
    tmp = head;
    while(tmp->next != NULL){
      if(tmp->alloc == 0){
	holes++;
      }
      tmp = tmp->next;
    }
  }
  return holes;
}

/* Get the number of bytes allocated */
int mem_allocated() {
  int allocated_bytes = 0;
  struct memoryList *tmp = NULL;
  
  /* Make sure head isn't null before we go through and accidentally segfault */
  if(head != NULL) {

    /* Gotta start at the head */
    if(head->alloc == 1){
      allocated_bytes += head->size;
    }

    /* Go through the memory */
    tmp = head;
    while(tmp->next != NULL){
      if(tmp->alloc == 1){
	allocated_bytes += tmp->size;
      }
      tmp = tmp->next;
    }
  }
  return allocated_bytes;
}

/* Number of non-allocated (a.k.a. free) bytes */
int mem_free() {
  int non_bytes = 0;
  struct memoryList *tmp = NULL;
  
  /* Make sure head isn't null before we go through and accidentally segfault */
  if(head != NULL) {

    /* Gotta start at the head */
    if(head->alloc == 0){
      non_bytes += head->size;
    }

    /* Go through the memory */
    tmp = head;
    while(tmp->next != NULL){
      if(tmp->alloc == 0){
	non_bytes += tmp->size;
      }
      tmp = tmp->next;
    }
  }
  return non_bytes;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free() {
  int free_bytes = 0;
  struct memoryList *tmp = NULL;
  
  /* Make sure head isn't null before we go through and accidentally segfault */
  if(head != NULL) {

    /* Gotta start at the head */
    if(head->alloc == 0){
      free_bytes = head->size;
    }

    /* Go through the memory */
    tmp = head;
    while(tmp->next != NULL){
      if(tmp->alloc == 0){
	if(tmp->size > free_bytes){
	  free_bytes = tmp->size;
	}
      }
      tmp = tmp->next;
    }
  }
  return free_bytes;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size) {
  int free_blocks = 0;
  struct memoryList *tmp = NULL;
    
  /* Make sure head isn't null before we go through and accidentally segfault */
  if(head != NULL) {

    /* Gotta start at the head */
    if(head->alloc == 0){
      if(head->size < size){
	free_blocks++;
      }
    }

    /* Go through the memory */
    tmp = head;
    while(tmp->next != NULL){
      if(tmp->alloc == 0){
	if(tmp->size < size){
	  free_blocks++;
	}
      }
      tmp = tmp->next;
    }
  }
  return free_blocks;
}       

char mem_is_alloc(void *ptr) {
  struct memoryList *tmp = head;
  
  while(tmp->next != NULL){
    if(tmp->ptr == ptr){
      return tmp->alloc;
    }
    tmp = tmp->next;
  }
  return 0;
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may ind them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool() {
  return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total() {
  return mySize;
}


// Get string name for a strategy. 
char *strategy_name(strategies strategy) {
  switch (strategy) {
    case Best:
      return "best";
    case Worst:
      return "worst";
    case First:
      return "first";
    case Next:
      return "next";
    default:
      return "unknown";
  }
}

// Get strategy from name.
strategies strategyFromString(char * strategy) {
  if (!strcmp(strategy,"best")) {
    return Best;
  } else if (!strcmp(strategy,"worst")) {
    return Worst;
  } else if (!strcmp(strategy,"first")) {
    return First;
  } else if (!strcmp(strategy,"next")) {
    return Next;
  } else {
    return 0;
  }
}


/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory() {
  return;
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */ 
void print_memory_status() {
  printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
  printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
  printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
  strategies strat;
  void *a, *b, *c, *d, *e;
  if(argc > 1)
    strat = strategyFromString(argv[1]);
  else
    strat = First;
	
	
  /* A simple example.  
     Each algorithm should produce a different layout. */
	
  initmem(strat,500);
	
  a = mymalloc(100);
  b = mymalloc(100);
  c = mymalloc(100);
  myfree(b);
  d = mymalloc(50);
  myfree(a);
  e = mymalloc(25);
	
  print_memory();
  print_memory_status();
}
