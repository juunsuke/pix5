#include "pix5net.h"


NetBuffer::NetBuffer(int max)
{
	_max = max;
	_buf = NULL;
	_pos = 0;
	_size = 0;
}

NetBuffer::~NetBuffer()
{
	if(_buf)
		free((void*)_buf);
}

int NetBuffer::make_room(int size, bool partial)
{
	// First check if there's already enough room
	if((_pos+_size+size)<=_alloc)
		return size;

	// Stop now if nothing can fit
	if(_size==_max)
		return 0;
	if(!partial && (_size+size)>_max)
		return 0;

	// Push left and see if it would fit
	if(_pos)
	{
		memmove((void*)_buf, (void*)(_buf+_pos), _size);
		_pos = 0;

		if((_size+size)<=_alloc)
			return size;
	}

	// Enlarge as much as we can
	_alloc = _size+size;
	if(_alloc>_max)
		_alloc = _max;

	_buf = (volatile uint8_t*)realloc((void*)_buf, _alloc);

	int have = _alloc-_size;
	return Math::min(have, size);
}

void NetBuffer::add(void *ptr, int size)
{
	//_mtx.lock();

	// Make room for the data
	if(!make_room(size, false))
	{
		// Failure
		//_mtx.unlock();
		E::NetBufferFull();
	}

	// Add the data
	memcpy((void*)(_buf+_pos+_size), ptr, size);
	_size += size;

	//_mtx.unlock();
}

int NetBuffer::try_add(void *ptr, int size, bool partial)
{
	//_mtx.lock();

	// Try to make room
	int room = make_room(size, partial);

	// Add as much as possible
	if(room)
	{
		memcpy((void*)(_buf+_pos+_size), ptr, room);
		_size += room;
	}

	//_mtx.unlock();

	return room;
}

