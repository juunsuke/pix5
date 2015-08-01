#pragma once


#define DEFAULT_FIFO_STRIDE			128

template<class T> class FIFO
{
	T *_buf;
	// Array

	int _first, _count;
	// First and size

	int _alloc;
	// Number of allocated elements in the buffer

	int _stride;
	// The stride

public:

	FIFO(int stride = DEFAULT_FIFO_STRIDE)
	{
		_buf = NULL;
		_first = 0;
		_count = 0;
		_alloc = 0;
		_stride = stride;
	}

	inline int count() const { return _count; }
	// Get the number of elements in the FIFO

	void add(T val)
	{
		// Add a new element into the FIFO

		// Push left ?
		if(_first && (_first+_count)==_alloc)
		{
			// Yes
			memmove(_buf, _buf+_first, _count*sizeof(T));
			_first = 0;
		}

		// Resize ?
		if(_count==_alloc)
		{
			// Yes
			_alloc += _stride;
			_buf = (T*)realloc(_buf, _alloc*sizeof(T));
		}

		// Append the element
		_buf[_first+_count] = val;
		_count++;
	}

	T get()
	{
		// Get an element from the FIFO list
		// Throws an error if none are available
		if(!_count)
			E::EmptyFIFO("FIFO is empty");

		// Remember the index
		int i = _first;

		// Adjust first/count
		if(_count==1)
		{
			_first = 0;
			_count = 0;
		}
		else
		{
			_first++;
			_count--;
		}

		// Return the element
		return _buf[i];
	}

	T peek() const
	{
		// Peek the first available element, without removing it
		// Throws an error if none are available
		if(!_count)
			E::EmptyFIFO("FIFO is empty");

		return _buf[_first];
	}

	void clear_nodel()
	{
		// Clear the buffer
		if(_buf)
		{
			free(_buf);
			_buf = NULL;
		}

		_first = 0;
		_count = 0;
	}

	void clear_del()
	{
		// Delete each element
		for(int c = _first; c<(_first+_count); c++)
			delete _buf[c];

		clear_nodel();
	}
};

