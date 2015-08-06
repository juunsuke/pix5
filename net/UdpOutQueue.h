#pragma once


class UdpOutQueue: public UdpQueue
{
	uint32_t _next_serial;
	// Next serial number
	
public:

	UdpOutQueue();
	~UdpOutQueue();

	void add(NetPacket *pak);
	// Add a packet to the queue
	// This will assign the packet a serial number and timestamp it

	NetPacket *get_expired(int64_t time, NetPacket *first = NULL);
	// Get the next expried packet, after 'first' and not including it
	// If 'first' is NULL, start from the beginning
	// The first packet with a timestamp lower than 'time' will be returned
	// Returns NULL when none is found
};

