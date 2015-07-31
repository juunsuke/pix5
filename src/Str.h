#pragma once


#define STR_SHORT_BUF_SIZE		256

class Str: public Obj
{
	PIXOBJ("Str")


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

	inline int comp(const Str& o) const				{ return strcmp(_buf, o._buf); }
	inline bool operator==(const Str& o) const		{ return comp(o)==0; }
	inline bool operator!=(const Str& o) const		{ return comp(o)!=0; }
	inline bool operator< (const Str& o) const		{ return comp(o)< 0; }
	inline bool operator<=(const Str& o) const		{ return comp(o)<=0; }
	inline bool operator> (const Str& o) const		{ return comp(o)> 0; }
	inline bool operator>=(const Str& o) const		{ return comp(o)>=0; }

	inline int comp(const char *o) const			{ return strcmp(_buf, o); }
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


	//
	// UTF8
	//

	uint32_t get_utf8_char(int pos, int &len) const
	{
		// Check if the next character is 1, 2, 3 or 4 bytes
		if(pos<0 || pos>=_len)
			return 0;

		char *src = _buf+pos;

		if(((*src)&0xF8) == 0xF0)
		{
			// 4 bytes
			// Validate the 2nd, 3rd and 4th bytes
			if((src[1]&0xC0) != 0x80)
				return 0;
			if((src[2]&0xC0) != 0x80)
				return 0;
			if((src[3]&0xC0) != 0x80)
				return 0;

			int b1 = (*src) & 0x07;
			int b2 = src[1] & 0x3F;
			int b3 = src[2] & 0x3F;
			int b4 = src[3] & 0x3F;

			len = 4;

			return (b1<<18) | (b2<<12) | (b3<<6) | b4;
		}

		if(((*src)&0xF0) == 0xE0)
		{
			// 3 bytes
			// Validate the 2nd abd 3rd byte
			if((src[1]&0xC0) != 0x80)
				return 0;
			if((src[2]&0xC0) != 0x80)
				return 0;

			int b1 = (*src) & 0x0F;
			int b2 = src[1] & 0x3F;
			int b3 = src[2] & 0x3F;

			len = 3;

			return (b1<<12) | (b2<<6) | b3;
		}

		if(((*src)&0xE0) == 0xC0)
		{
			// 2 bytes
			// Validate the 2nd byte
			if((src[1]&0xC0) != 0x80)
				return 0;

			int b1 = (*src) & 0x1F;
			int b2 = src[1] & 0x3F;

			len = 2;

			return (b1<<6) | b2;
		}

		if(((*src)&0x80) == 0)
		{
			// 1 byte
			len = 1;

			return *src;
		}

		// Error
		return 0;
	}

	int get_utf8_len() const
	{
		// Return the number of characters in an UTF8 string
		int tot = 0;
		int pos = 0;
		int len;
		while(get_utf8_char(pos, len))
		{
			tot++;
			pos += len;
		}

		return tot;
	}

	int get_utf8_start(int i) const
	{
		// Return the starting offset of the i'th unicode character
		// Returns -1 if 'i' is an invalid index
		int pos = 0;
		for(int c = 0; c<i; c++)
		{
			int len;
			if(!get_utf8_char(pos, len))
				return -1;

			pos += len;
		}

		return pos;
	}

};

