#include "pix5.h"

namespace PIX {



#ifdef DBG


class Stub: public Obj
{
	PIXOBJ("Stub")
};


static Mutex _mtx;
// The mutex used for addition/removal of objects

static Obj *_first = NULL;
static Obj *_last = NULL;


void obj_init() __attribute__((constructor(101)));
void obj_done() __attribute__((destructor(101)));


void obj_init()
{
	// Create the first/last pointers
	_first = new Stub();
	_last = new Stub();

	_first->__obj_next = _last;
	_first->__obj_prev = NULL;

	_last->__obj_next = NULL;
	_last->__obj_prev = _first;
}

void obj_done()
{
	// Look for memory leaks
	for(Obj *run = _first->__obj_next; run!=_last; run = run->__obj_next)
		fprintf(stderr, "Instance not freed: %s\n", run->__obj_get_name());
}


Obj::Obj()
{
	// Link the object in the linked list
	if(!_first || !_last)
		return;

	_mtx.lock();

	// Link it
	__obj_prev = _first;
	__obj_next = _first->__obj_next;

	__obj_prev->__obj_next = this;
	__obj_next->__obj_prev = this;

	_mtx.unlock();
}

Obj::~Obj()
{
	if(!_first || !_last)
		return;

	_mtx.lock();

	__obj_prev->__obj_next = __obj_next;
	__obj_next->__obj_prev = __obj_prev;

	__obj_prev = NULL;
	__obj_next = NULL;

	_mtx.unlock();
}

#endif




}
