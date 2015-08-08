#pragma once


template<class T> class Buffer
{
	T *_ptr;
	// Actual buffer pointer

	int _size;
	// Size, in elements


	void copy(const Buffer<T>& o)
	{
		_size = o._size;

		if(_size)
		{
			_ptr = (T*)malloc(_size*sizeof(T));
			memcpy(_ptr, o._ptr, _size*sizeof(T));
		}
		else
			_ptr = NULL;
	}

public:

	Buffer()
	{
		// Empty buffer
		_ptr = NULL;
		_size = 0;
	}

	Buffer(const Buffer<T>& o)
	{
		// Copy constructor
		copy(o);
	}

	Buffer(const Buffer *o)
	{
		// Copy from pointer
		copy(*o);
	}

	Buffer(int size, bool clear = false)
	{
		// Pre-allocate the buffer
		if(size<0)
			size = 0;

		_ptr = (T*)malloc(size*sizeof(T));
		_size = size;

		if(clear)
			this->clear();
	}

	~Buffer()
	{
		free();
	}

	void free()
	{
		// Free the buffer
		if(_ptr)
		{
			::free(_ptr);
			_ptr = NULL;
		}

		_size = 0;
	}

	inline int size() const { return _size; }
	// Get the size

	inline const T* ptr_const() const { return _ptr; }
	inline operator const T*() const { return _ptr; }
	inline T* ptr() { return _ptr; }
	inline operator T*() { return _ptr; }
	// Get the pointer, which is valid until the buffer is resized or freed

	void resize(int size, bool clear = false)
	{
		// Reize the buffer
		if(size<0)
			size = 0;

		if(size==_size)
			return;

		// Resize
		_ptr = (T*)realloc(_ptr, size*sizeof(T));

		// Clear
		if(clear && size>_size)
			memset(_ptr+_size, 0, (size-_size)*sizeof(T));
		
		_size = size;
	}

	inline T& operator[](int i)
	{
		ASSERT(i>=0 && i<_size, "Buffer::operator[]: Index out of bounds")

		return _ptr[i];
	}

	Buffer& operator=(const Buffer<T>& o)
	{
		// Affectation
		free();
		copy(o);
		return *this;
	}

	void copy(int di, const Buffer<T>& src, int si, int size)
	{
		// Copy data from another buffer
		// si: index in the source buffer where to start reading
		// di: index in this buffer where to start writing
		// size: number of elements to copy
		if(size<=0)
			return;

		ASSERT(di>=0 && (di+size)<=_size, "Buffer::copy(): Destination range is out of bounds")
		ASSERT(si>=0 && (si+size)<=src._size, "Buffer::copy(): Source range is out of bounds")

		// Do the copy
		memmove(_ptr+di, src->_ptr+si, size*sizeof(T));
	}

	void copy(int di, T* src, int size)
	{
		// Copy data from a pointer
		// di: index in this buffer where to start writing
		// src: Source pointer
		// size: number of elements to copy
		
		if(size<=0)
			return;

		ASSERT(di>=0 && (di+size)<=_size, "Buffer::copy(): Destination range is out of bounds")

		memmove(_ptr+di, src, size*sizeof(T));
	}

	void cut(int start, int size)
	{
		// Cut 'size' elements starting at index 'start'
		// Everything to the right of this range will be moved to the left
		if(size<=0)
			return;

		ASSERT(start>=0 && (start+size)<=_size, "Buffer::cut(): Range is out of bounds")

		// Move left
		if((start+size)<_size)
			memmove(_ptr+start, _ptr+start+size, (_size-start-size)*sizeof(T));
	}

	void insert(int start, int size)
	{
		// Make room for 'size' elements starting at index 'start'
		// Everything will be pushed right
		if(size<=0)
			return;

		ASSERT(start>=0 && (start+size)<=_size, "Buffer::insert(): Range is out of bounds")

		// Move right
		if((start+size)<_size)
			memmove(_ptr+start+size, _ptr+start, (_size-start-size)*sizeof(T));
	}

	void clear(int ch = 0)
	{
		// Clear the buffer
		memset(_ptr, ch, _size*sizeof(T));
	}
};

