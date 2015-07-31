#include "pix5.h"

#ifdef DBG
#ifndef NO_OVERLOAD_NEW_DELETE

void dbg_init() __attribute__((constructor(101)));
void dbg_done() __attribute__((destructor(101)));


#undef new


typedef struct MemBlock
{
	size_t size;
	const char *file;
	int line;
	struct MemBlock *self;

	struct MemBlock *prev, *next;
} MemBlock;


static MemBlock *_first = NULL;
static MemBlock *_last = NULL;

static Mutex _mtx;


void dbg_init()
{
	// Allocate the first and last linked list entries
	_first = (MemBlock*)calloc(1, sizeof(MemBlock));
	_last = (MemBlock*)calloc(1, sizeof(MemBlock));

	_first->next = _last;
	_last->prev = _first;
}

void dbg_done()
{
	// Check for memory leaks
	for(MemBlock *mb = _first->next; mb!=_last; mb = mb->next)
		fprintf(stderr, "%s: %i: Undeleted new (%i bytes)\n", mb->file, mb->line, (int)mb->size);
}

void *operator new(size_t size, const char *file, int line)
{
	// Allocate a block and debug-clear it
	MemBlock *mb = (MemBlock*)malloc(sizeof(MemBlock)+size+sizeof(MemBlock*));
	memset(mb+1, 0xBD, size);

	mb->size = size;
	mb->file = file;
	mb->line = line;
	mb->self = mb;
	*(MemBlock**)(((char*)mb)+sizeof(MemBlock)+size) = mb;

	// Insert it in the doubly linked list
	_mtx.lock();

	mb->prev = _first;
	mb->next = _first->next;
	mb->prev->next = mb;
	mb->next->prev = mb;

	_mtx.unlock();

	return mb+1;
}
	
void new_delete(void *ptr)
{
	// Don't choke on NULL
	if(!ptr)
		return;

	// Validate the block
	MemBlock *mb = ((MemBlock*)ptr)-1;

	if(mb->self!=mb)
		E::Assert("new_delete(): Invalid pointer");

	if(*(MemBlock**)(((char*)mb)+sizeof(MemBlock)+mb->size) != mb)
		E::Assert("new_delete(): Overflow detected");

	// Unlink it
	_mtx.lock();

	mb->prev->next = mb->next;
	mb->next->prev = mb->prev;

	_mtx.unlock();

	free(mb);
}

void operator delete(void *ptr)
{
	// Don't choke on NULL
	if(!ptr)
		return;

	// Validate the block
	MemBlock *mb = ((MemBlock*)ptr)-1;

	if(mb->self!=mb)
		E::Assert("delete(): Invalid pointer");

	if(*(MemBlock**)(((char*)mb)+sizeof(MemBlock)+mb->size) != mb)
		E::Assert("delete(): Overflow detected");

	// Unlink it
	_mtx.lock();

	mb->prev->next = mb->next;
	mb->next->prev = mb->prev;

	_mtx.unlock();


	free(mb);
}


#endif
#endif

