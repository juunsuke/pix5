#pragma once


#define MAX_NETBUFFER_SIZE			(1024*1024)
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

	//Mutex _mtx;
	// The buffer's mutex

	int make_room(int size, bool partial);
	// Make room in the buffer to hold 'size' bytes

public:

	NetBuffer(int max = MAX_NETBUFFER_SIZE);
	~NetBuffer();

	virtual int max_size() { return _max; }
	// Get the max size

	virtual int size() { return _size; }
	// Get the number of queued bytes


	void add(void *ptr, int size);
	// Add data to the buffer
	// There must be enough room to fit the data, otherwise an error will be thrown

	int try_add(void *ptr, int size, bool partial = false);
	// Try to add data to the buffer, if there is enough room
	// If there is enough room, everything is added and 'size' is returned
	// If there is not enough room and 'partial' is false, nothing is added and 0 is returned
	// If there is not enough room and 'partial' is true, as much as fits in the buffer is added
	// and the number of added bytes is returned
};


