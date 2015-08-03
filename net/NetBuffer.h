#pragma once


//#define MAX_NETBUFFER_SIZE			(1024*1024)
#define MAX_NETBUFFER_SIZE			100
// 1MB default buffer size
// That means 2MB max per client
// 2GB for 1,000 clients at max buffer size



class NetBuffer
{
	int _max;
	// Max buffer size

	volatile uint8_t *_buf;
	// The actual buffer

	volatile int _pos, _size;
	// Current position and size within the buffer

	volatile int _alloc;
	// Currently allocated bytes

	Mutex _mtx;
	// The buffer's mutex

	int make_room(int size, bool partial);
	// Make room in the buffer to hold 'size' bytes

	int peek_get(void *ptr, int pos, int size, bool partial, bool remove);
	// Peek+Get main body
	
	
public:

	NetBuffer(int max = MAX_NETBUFFER_SIZE);
	~NetBuffer();

	virtual int max_size() { return _max; }
	// Get the max size

	virtual int size() { return _size; }
	// Get the number of queued bytes

	virtual int avail() { return _max-_size; }
	// Return the maximum available size in the buffer

	
	int add(const void *ptr, int size, bool partial = false);
	// Try to add data to the buffer, if there is enough room
	// If there is enough room, everything is added and 'size' is returned
	// If there is not enough room and 'partial' is false, nothing is added and 0 is returned
	// If there is not enough room and 'partial' is true, as much as fits in the buffer is added
	// and the number of added bytes is returned
	// This function never throws any error

	int peek(void *ptr, int pos, int size);
	// Peek some data without removing it from the buffer
	// 'pos' is the starting position within the buffer and 'size' is the number of bytes
	// As many bytes as possible will be copied to the buffer pointed to by 'ptr'
	// Returns the number of copied bytes
	// If any value is out of range, or no data is available, 0 will be returned
	// This function never throws any error

	int get(void *ptr, int size, bool partial);
	// Retrieve some data from the beginning of the buffer
	// If 'partial' is false, and there is less than 'size' bytes availale in the
	// buffer, 0 will be returned and nothing will be copied
	// If 'partial' is true, and there is less than 'size' bytes available in the
	// buffer, all the remaining data will be copied and the number of read bytes
	// will be returned
	// If the buffer is larger than 'size', 'size' bytes will be copied and 'size'
	// will  be returned
	// If any value is out of range, or no data is available, 0 will be returned
	// This function never throws any error

};


