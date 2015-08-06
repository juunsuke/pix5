#include "pix5net.h"


UdpInQueue::UdpInQueue()
{
	_last_serial = 0;
}

UdpInQueue::~UdpInQueue()
{
}

void UdpInQueue::add(NetPacket *pak)
{
	// If it's a copy of an already handled packet, drop it
	if(pak->_data.serial<=_last_serial)
	{
		// Drop it
		Net::recycle_packet(pak);
		return;
	}

	// Check the queue
	for(NetPacket *run = _first->_next; run!=_last; run = run->_next)
	{
		// If we already have a packet with this serial, drop it
		if(run->_data.serial==pak->_data.serial)
		{
			// Drop it
			Net::recycle_packet(pak);
			return;
		}

		// Insert it if this is where it should be
		if(run->_data.serial>pak->_data.serial)
		{
			// Add it before
			UdpQueue::add(pak, run);
			return;
		}
	}

	// If we got here, it means the packet should be at the end of the queue
	UdpQueue::add(pak);
}

NetPacket *UdpInQueue::get()
{
	// Check if there is a packet in queue
	NetPacket *pak = _first->_next;
	
	if(pak==_last)
		// No
		return NULL;

	// Is it the next in sequence ?
	if(pak->_data.serial==(_last_serial+1))
	{
		// Yes, grab it
		remove(pak, false);
		_last_serial++;

		return pak;
	}

	// No, missing one
	return NULL;
}

bool UdpInQueue::has_message()
{
	// Check if all the packets of a message are received
	NetPacket *pak = _first->_next;

	if(pak==_last)
		return false;

	if(pak->_data.serial>(_last_serial+1))
		return false;

	// First packet in the queue is the first packet of a message
	// Check if they are all there
	int num = pak->_data.flags & 0xFF;
	for(int c = 0; c<num-1; c++)
	{
		pak = pak->_next;

		if(pak==_last || pak->_data.serial!=(pak->_prev->_data.serial+1))
			return false;
	}

	// All packets are there
	return true;
}

