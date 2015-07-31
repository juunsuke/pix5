#pragma once


//
// Sorted list
// <T> must be a pointer type, usually a class, that implements the comp() function :
// int comp(T* o);
// Which returns -1, 0, or 1, depending of if it's lesser, equal, or greater than 'o', respectively
//

template<class T> class SortedList: public Obj
{
	PIXOBJ("SortedList")

	List<T> _list;
	// The actual list we're wrapping

	void copy(const SortedList<T>& o)
	{
		// Copy the list
		_list = o._list;
	}
	
	inline void check_index(int index, const char *func, bool allow_end = false) const
	{
		#ifdef DBG
			// Validate an index
			if(index<0 || (!allow_end && index>=_list.size()) || (allow_end && index>_list.size()))
				E::OutOfBounds("SortedList<%s>::%s(): Index out of bounds.  Index:%i  Size:%i", demangle(typeid(T).name()).ptr(), func, index, _list.size());
		#endif
	}

public:

	SortedList(int stride = DEFAULT_LIST_STRIDE)
	{
		// Empty list
		if(stride!=DEFAULT_LIST_STRIDE)
			_list.set_stride(stride);
	}

	SortedList(const SortedList<T>& o)
	{
		// Copy constructor
		copy(o);
	}

	~SortedList()
	{
		#ifdef DBG
		if(_list.size())
			Log::debug("SortedList<%s>::~SortedList(): The list still contains %i elements", demangle(typeid(T).name()).ptr(), _list.size());
		#endif

		_list.clear_nodel();
	}

	SortedList<T>& operator=(const SortedList<T>& o)
	{
		// Affectation
		#ifdef DBG
		if(_list.size())
			Log::debug("SortedList<%s>::~operator=(): The list still contains %i elements", demangle(typeid(T).name()).ptr(), _list.size());
		#endif

		_list.clear_nodel();

		copy(o);
		return *this;
	}


	void set_stride(int stride)
	{
		// Change the stride
		ASSERT(stride>=1, "SortedList<>::set_stride(): Invalid stride")
		_list.set_stride(stride);
	}

	int stride() const { return _list.stride(); }
	// Retrieve the stride

	inline int size() const { return _list.size(); };
	// Get the size of the list

	inline T get(int index) const
	{
		// Get an element
		check_index(index, "get");
		return _list.get(index);
	}

	inline T operator[](int index)
	{
		check_index(index, "operator[]");
		return _list.get(index);
	}

	void clear_nodel()
	{
		// Clear the list
		_list.clear_nodel();
	}

	void clear_del()
	{
		// Clear the list
		_list.clear_del();
	}

	int find(T val) const
	{
		// Find 'val' in the list, or where it should go, using a binary search
		//
		// If the exact element is found, its index is returned
		// Otherwise, a negative value is returned
		// The absolute value of that value, minus one, is the index where the
		// value should be inserted to be properly sorted
		// So, if find() returns -8, then the value should be inserted at index 7
		//
		// All comparisons are made on (*val), not on the (val) pointer itself
		//
		// If the list contains more than one element that would be equal to 'val',
		// (which, since sorted, would be contiguous), the first found index will
		// be returned, which will not necessarily be the first or last of the group

		// First check corner cases

		// Empty list
		if(_list.size()==0)
			return -1;

		// Perform the binary search
		int left = 0;
		int right = _list.size()-1;

		// Continue until only one element is left
		while(right>left)
		{
			// Check the middle value
			int mid = left + (right-left)/2;
			int res = val->comp(_list.get(mid));

			if(res==0)
				return mid;

			if(res<0)
				right = mid-1;
			else
				left = mid+1;
		}

		// Only one element left to check
		int res = val->comp(_list.get(left));

		if(res==0)
			return left;

		if(res<0)
			return -(left+1);

		return -(left+2);
	}

	int find_exact(T val) const
	{
		// Find the exact pointer 'val' in the list and return its index
		// Returns -1 if 'val' is not in the list

		// First do a bit search to find the range
		int i = find(val);
		if(i<0)
			return -1;

		// Look for the exact value
		// Look i and left
		for(int c = i; c>=0; c--)
		{
			T v2 = _list.get(c);

			if(v2==val)
				return c;

			if(val->comp(v2)!=0)
				break;
		}

		// Look right
		for(int c = i+1; c<_list.size(); c++)
		{
			T v2 = _list.get(c);

			if(v2==val)
				return c;

			if(val->comp(v2)!=0)
				break;
		}

		// Not found
		return -1;
	}

	void add(T val)
	{
		// Find where the element should go
		int i = find(val);

		if(i<0)
			i = (-i)-1;

		// Insert it
		_list.insert(val, i);
	}

	void remove_nodel(int index, int count = 1)
	{
		// Remove an element from the list, without deleting it
		check_index(index, "remove_nodel");
		check_index(index+count, "remove_nodel", true);

		_list.remove_nodel(index, count);
	}

	inline void remove_del(int index, int count = 1)
	{
		// Remove an element from the list, deleting it
		check_index(index, "remove_del");
		check_index(index+count, "remove_del", true);

		_list.remove_del(index, count);
	}

};



