#pragma once


class NetMessage
{
	Buffer<uint8_t> _buf;
	// Raw data buffer

	int _size;
	// Actual message size

	int _pos;
	// Current read/write position


	void copy(const NetMessage& o);
	// Copy a message

	void add_size(int sz);
	// Make sure there is enough room for 'sz' bytes ahead of the position
	// Also increase the message size

	void has_size(int sz);
	// Make sure the message has that many bytes left to read
	// Throw an error if not

public:

	NetMessage();
	~NetMessage();

	NetMessage(const NetMessage& o)
	{
		copy(o);
	}

	NetMessage(NetMessage *o)
	{
		copy(*o);
	}

	NetMessage& operator=(const NetMessage& o)
	{
		if(_buf)
			free(_buf);

		copy(o);
		return *this;
	}

	NetMessage& operator=(NetMessage *o)
	{
		if(_buf)
			free(_buf);

		copy(*o);
		return *this;
	}

	inline int size() { return _size; }
	inline int pos() { return _pos; }
	inline uint8_t* ptr() { return _buf; }
	// Get access to the message data

	void reset_pos() { _pos = 0; }
	// Reset the position to 0


	//
	// Message construction
	// Add values of different types to the message
	//

	void add_i8(int8_t val);
	void add_u8(uint8_t val);
	void add_i16(int16_t val);
	void add_u16(uint16_t val);
	void add_i32(int32_t val);
	void add_u32(uint32_t val);
	void add_i64(int64_t val);
	void add_u64(uint64_t val);
	// Add an integer to the message

	void add_float(float val);
	void add_double(double val);
	// Add a floating point value to the message

	void add_str(const Str& val);
	// Add a string to the message
	// Max length for a string is 65,535

	void add_raw(const void *ptr, int size);
	// Add raw data to the message


	//
	// Message reading/decoding
	// Read values of different types, sequentially, from the message
	// Trying to read a value beyond the end of the message, or of the
	// wrong type will throw an E::NetMessage error
	//

	bool end();
	// Returns true if the end of the message was reached
	// Useful for messages of varying size

	int8_t   get_i8();
	uint8_t  get_u8();
	int16_t  get_i16();
	uint16_t get_u16();
	int32_t  get_i32();
	uint32_t get_u32();
	int64_t  get_i64();
	uint64_t get_u64();
	// Get an integer value from the message

	float get_float();
	double get_double();
	// Get a floating point value from the message

	Str get_str();
	// Get a string from the message

	void get_raw(void *ptr, int size);
	// Get raw data from the message

};

