#pragma once


class UdpQueue
{
protected:

	NetPacket *_first, *_last;
	// First/last in the doubly linked list

	int _size;
	// Number of packets in the queue

public:

	UdpQueue();
	~UdpQueue();

	inline int size() { return _size; }
	// Get the size

	void add(NetPacket *pak, NetPacket *before = NULL);
	// Add a packet to the queue

	void remove(NetPacket *pak, bool recycle = true);
	// Remove a packet from the queue

	void remove(uint32_t serial);
	// Remove the packet with the given serial from the queue, if it's there
	// Return silently if it's not there
};

