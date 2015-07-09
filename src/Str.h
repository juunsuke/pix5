#pragma once


#define STR_SHORT_BUF_SIZE		256

class Str
{

	int _len;
	// Current string length

	char *_buf;
	// Current buffer

	char _short_buf[STR_SHORT_BUF_SIZE];
	// Static short buffer to avoid allocation for short quick strings


	inline void size(int sz)
	{
		// Set the size of the string
		_len = sz;

		if(sz<STR_SHORT_BUF_SIZE)
			_buf = _short_buf;
		else
			_buf = (char*)malloc(sz+1);

		// Add the terminal 0
		_buf[sz] = 0;
	}

	inline void clear()
	{
		// Free if needed
		if(_buf!=_short_buf)
			free(_buf);
	}

	void append(const char *o, int len)
	{
		// Append data to ourselves
		if((_len+len)<STR_SHORT_BUF_SIZE)
		{
			// All fit in short buffer
			memcpy(_buf+_len, o, len+1);
			_len += len;
		}
		else if(_buf==_short_buf)
		{
			// Going from short to long buffer
			int old_len = _len;
			size(old_len+len);

			memcpy(_buf, _short_buf, old_len);
			memcpy(_buf+old_len, o, len);
		}
		else
		{
			// Enlarging a long buffer
			_buf = (char*)realloc(_buf, _len+len+1);
			memcpy(_buf+_len, o, len+1);
			_len += len;
		}
	}

	int do_find(const char *str, int len, int start) const
	{
		// Look for the sub-string
		for(int c = start; (c+len)<=_len; c++)
			if(strncmp(_buf+c, str, len)==0)
				return c;

		// Not found
		return -1;
	}

public:


	inline Str()
	{
		// Empty string
		size(0);
	}

	inline Str(const Str& o)
	{
		// Copy constructor
		size(o._len);
		memcpy(_buf, o._buf, _len);
	}

	inline Str(const char *o)
	{
		// Copy a C string
		size(strlen(o));
		memcpy(_buf, o, _len);
	}

	inline Str(const char *o, int len)
	{
		// Copy a buffer with given size
		size(len);
		memcpy(_buf, o, len);
	}

	inline ~Str()
	{
		clear();
	}


	inline Str& operator=(const Str& o)
	{
		// Affectation
		clear();
		size(o._len);
		memcpy(_buf, o._buf, _len);

		return *this;
	}

	inline Str& operator=(const char *o)
	{
		// Affectation
		clear();
		size(strlen(o));
		memcpy(_buf, o, _len);

		return *this;
	}


	inline const char *ptr() const
	{
		// Get a pointer, valid until an affectation is done
		return _buf;
	}

	inline operator const char*() const
	{
		// Get a pointer, valid until an affectation is done
		return _buf;
	}

	inline int len() const
	{
		// Get the length
		return _len;
	}

	inline char operator[](int index) const
	{
		// Return a character from the string
		if(index<0 || index>_len)
			return 0;
		else
			return _buf[index];
	}



	//
	// Comparisons
	//

	inline bool comp(const Str& o) const			{ return strcmp(_buf, o._buf); }
	inline bool operator==(const Str& o) const		{ return comp(o)==0; }
	inline bool operator!=(const Str& o) const		{ return comp(o)!=0; }
	inline bool operator< (const Str& o) const		{ return comp(o)< 0; }
	inline bool operator<=(const Str& o) const		{ return comp(o)<=0; }
	inline bool operator> (const Str& o) const		{ return comp(o)> 0; }
	inline bool operator>=(const Str& o) const		{ return comp(o)>=0; }

	inline bool comp(const char *o) const			{ return strcmp(_buf, o); }
	inline bool operator==(const char *o) const		{ return comp(o)==0; }
	inline bool operator!=(const char *o) const		{ return comp(o)!=0; }
	inline bool operator< (const char *o) const		{ return comp(o)< 0; }
	inline bool operator<=(const char *o) const		{ return comp(o)<=0; }
	inline bool operator> (const char *o) const		{ return comp(o)> 0; }
	inline bool operator>=(const char *o) const		{ return comp(o)>=0; }


	//
	// Concatenations
	//

	Str operator+(const Str& o) const
	{
		Str res;
		res.size(_len+o._len);
		memcpy(res._buf, _buf, _len);
		memcpy(res._buf+_len, o._buf, o._len);

		return res;
	}

	Str operator+(const char *o) const
	{
		int olen = strlen(o);

		Str res;
		res.size(_len+olen);
		memcpy(res._buf, _buf, _len);
		memcpy(res._buf+_len, o, olen);

		return res;
	}

	Str& operator+=(const Str& o)
	{
		// Concatenate to ourselves
		append(o._buf, o._len);
		return *this;
	}

	Str& operator+=(const char *o)
	{
		// Concatenate to ourselves
		append(o, strlen(o));
		return *this;
	}



	//
	// Transformations
	//

	Str toupper() const
	{
		Str res;
		res.size(_len);

		for(int c = 0; c<_len; c++)
			res._buf[c] = ::toupper(_buf[c]);

		return res;
	}

	Str tolower() const
	{
		Str res;
		res.size(_len);

		for(int c = 0; c<_len; c++)
			res._buf[c] = ::tolower(_buf[c]);

		return res;
	}


	//
	// Sub-sections
	//

	Str sub(int start, int len) const
	{
		// Get a sub-section
		if(start<0)
			start = _len+start;

		if(start<0)
		{
			len += start;
			start = 0;
		}

		if(start>=_len)
			return Str();

		if((start+len)>_len)
			len = _len-start;

		if(len<=0)
			return Str();

		Str res;
		res.size(len);
		memcpy(res._buf, _buf+start, len);

		return res;
	}

	Str sub(int start) const
	{
		// Get a subsection
		if(start<0)
		{
			start = _len+start;
	
			if(start<0)
				start = 0;
		}

		return sub(start, _len-start);
	}

	Str left(int len) const
	{
		return sub(0, len);
	}

	Str right(int len) const
	{
		if(len<=0)
			return Str();

		return sub(-len);
	}


	//
	// Searches
	//

	int find(const Str& o, int start = 0) const
	{
		// Find a sub-string
		return do_find(o._buf, o._len, start);
	}

	int find(const char *o, int start = 0) const
	{
		// Find a sub-string
		return do_find(o, strlen(o), start);
	}

	int find(const char *o, int len, int start) const
	{
		// Find a sub-string
		return do_find(o, len, start);
	}

	int find(char ch, int start = 0) const
	{
		return find(&ch, 1, start);
	}



	//
	// String building
	//

	static Str buildv(const char *fmt, va_list vl)
	{
		// Build a string
		Str res;
		va_list vl2;
		va_copy(vl2, vl);
		res.size(vsnprintf(NULL, 0, fmt, vl2));

		vsprintf(res._buf, fmt, vl);

		return res;
	}

	static Str build(const char *fmt, ...)
	{
		va_list vl;
		va_start(vl, fmt);
		return buildv(fmt, vl);
	}
};
