#include "pix5net.h"


UdpQueue::UdpQueue()
{
	_size = 0;

	// Initialize the doubly linked list
	_first = new NetPacket();
	_last = new NetPacket();

	_first->_next = _last;
	_first->_prev = NULL;

	_last->_next = NULL;
	_last->_prev = _first;
}

UdpQueue::~UdpQueue()
{
	// Free all the packets
	for(NetPacket *run = _first; run;)
	{
		// Recycle the packet
		NetPacket *next = run->_next;
		Net::recycle_packet(run);

		run = next;
	}
}

void UdpQueue::add(NetPacket *pak, NetPacket *before)
{
	// Link the new packet
	if(!before)
		before = _last;

	pak->_prev = before->_prev;
	pak->_next = before;

	pak->_prev->_next = pak;
	pak->_next->_prev = pak;

	_size++;
}

void UdpQueue::remove(NetPacket *pak, bool recycle)
{
	// Unlink a packet
	pak->_prev->_next = pak->_next;
	pak->_next->_prev = pak->_prev;

	pak->_prev = NULL;
	pak->_next = NULL;

	// Recycle it
	if(recycle)
		Net::recycle_packet(pak);

	_size--;
}
	
void UdpQueue::remove(uint32_t serial)
{
	// Look for the packet with the given serial
	for(NetPacket *run = _first->_next; run!=_last && run->_data.serial<=serial; run = run->_next)
		if(run->_data.serial==serial)
		{
			// Found it
			remove(run);
			return;
		}
}


