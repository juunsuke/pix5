#pragma once


class UdpInQueue: public UdpQueue
{
	uint32_t _last_serial;
	// The last properly handled packet serial

public:

	UdpInQueue();
	~UdpInQueue();


	void add(NetPacket *pak);
	// Add a received packet to the queue

	NetPacket *get();
	// Get the next packet in the porper order
	// Returns NULL if not available

	bool has_message();
	// Returns true if all packets of a message are ready to be retrieved
};

