#pragma once


#define DEFAULT_LIST_STRIDE			128


template<class T> class List
{
	Buffer<T> _buf;
	// Actual array buffer

	int _size;
	// Number of elements in the list

	int _stride;
	// List stride, in elements


	void copy(const List<T>& o)
	{
		// Copy another list
		_size = o._size;
		_stride = o._stride;
		_buf = o._buf;
	}

	inline void check_index(int index, const char *func, bool allow_end = false) const
	{
		#ifdef DBG
			// Validate an index
			if(index<0 || (!allow_end && index>=_size) || (allow_end && index>_size))
				E::OutOfBounds("List<%s>::%s(): Index out of bounds.  Index:%i  Size:%i", demangle(typeid(T).name()).ptr(), func, index, _size);
		#endif
	}

public:

	List(int stride = DEFAULT_LIST_STRIDE)
	{
		// Empty list
		_size = 0;
		_stride = stride;
	}

	List(const List<T>& o)
	{
		// Copy constructor
		copy(o);
	}

	~List()
	{
		#ifdef DBG
		if(_size)
			Log::debug("List<%s>::~List(): The list still contains %i elements", demangle(typeid(T).name()).ptr(), _size);
		#endif
	}

	List<T>& operator=(const List<T>& o)
	{
		// Affectation
		#ifdef DBG
		if(_size)
			Log::debug("List<%s>::~operator=(): The list still contains %i elements", demangle(typeid(T).name()).ptr(), _size);
		#endif

		copy(o);
		return *this;
	}


	void set_stride(int stride)
	{
		// Change the stride
		ASSERT(stride>=1, "List<>::set_stride(): Invalid stride")

		_stride = stride;
	}

	int stride() const { return _stride; }
	// Retrieve the stride

	inline int size() const { return _size; };
	// Get the size of the list

	inline T get(int index) const
	{
		// Get an element
		check_index(index, "get");
		return _buf[index];
	}

	inline void set(int index, T val)
	{
		// Set an element
		check_index(index, "set");
		_buf[index] = val;
	}

	inline T& operator[](int index)
	{
		// Return a reference to an element
		// The returned reference can be considered valid until the next call to any List function
		// As such, references should not be kept, but rather used immediatly and discarded
		check_index(index, "operator[]");
		return _buf[index];
	}

	void clear_nodel()
	{
		// Clear the list without deleting entries
		_buf.free();
		_size = 0;
	}

	void clear_del()
	{
		// Delete every elements
		for(int c = 0; c<_size; c++)
			delete _buf[c];

		// Clear the buffer
		clear_nodel();
	}

	int insert(T val, int index)
	{
		// Insert a value in the list at the given index
		// Returns 'index'

		check_index(index, "insert", true);

		// Enlarge the buffer ?
		if(_size==_buf.size())
			_buf.resize(_buf.size()+_stride);

		// Move data to the right
		_buf.insert(index, 1);

		// Set the new element
		_buf[index] = val;
		_size++;

		return index;
	}

	inline int add(T val)
	{
		// Add an element at the end of the list
		return insert(val, _size);
	}

	void remove_nodel(int index, int count = 1)
	{
		// Remove an element from the list, without deleting it
		check_index(index, "remove_nodel");
		check_index(index+count, "remove_nodel", true);

		// Move data to the left
		_buf.cut(index, 1);

		_size -= count;
	}

	inline void remove_del(int index, int count = 1)
	{
		// Remove an element from the list, deleting it
		check_index(index, "remove_del");
		check_index(index+count, "remove_del", true);

		for(int c = 0; c<count; c++)
			delete _buf[index+c];

		remove_nodel(index, count);
	}

	int find(const T& val, int start = 0) const
	{
		// Find the first instance of the given value within the list
		for(int c = start; c<_size; c++)
			if(_buf[c]==val)
				return c;

		// Not found
		return -1;
	}
};



