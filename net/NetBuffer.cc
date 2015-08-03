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

int NetBuffer::add(const void *ptr, int size, bool partial)
{
	_mtx.lock();

	// Try to make room
	int room = make_room(size, partial);

	// Add as much as possible
	if(room)
	{
		memcpy((void*)(_buf+_pos+_size), ptr, room);
		_size += room;
	}

	_mtx.unlock();

	return room;
}

int NetBuffer::peek_get(void *ptr, int pos, int size, bool partial, bool remove)
{
	_mtx.lock();

	// First check for no-data cases
	if(pos<0 || pos>=_size || size<=0)
	{
		_mtx.unlock();
		return 0;
	}

	// Adjust the size
	if((pos+size)>_size)
	{
		// If no partial is allowed, returned 0
		if(!partial)
		{
			_mtx.unlock();
			return 0;
		}

		size = _size-pos;
	}

	// Copy the data
	memcpy(ptr, (void*)(_buf+_pos+pos), size);

	if(remove)
	{
		_size -= size;

		if(_size)
			_pos += size;
		else
			_pos = 0;
	}

	_mtx.unlock();

	return size;
}

int NetBuffer::peek(void *ptr, int pos, int size)
{
	// Peek some data
	return peek_get(ptr, pos, size, true, false);
}

int NetBuffer::get(void *ptr, int size, bool partial)
{
	// Get some data
	return peek_get(ptr, 0, size, partial, true);
}

