#include "pix5net.h"


UdpOutQueue::UdpOutQueue()
{
	_next_serial = 0;
}

UdpOutQueue::~UdpOutQueue()
{
}

void UdpOutQueue::add(NetPacket *pak)
{
	// Set the serial and timestamp
	pak->_data.serial = _next_serial++;
	pak->_ts = get_ms();

	// Add it
	UdpQueue::add(pak);
}

NetPacket *UdpOutQueue::get_expired(int64_t time, NetPacket *first)
{
	// Start from the beginning ?
	if(!first)
		first = _first;

	// Look for the first expired packet
	for(NetPacket *run = first->_next; run!=_last; run = run->_next)
		if(run->_ts<time)
			return run;

	// None found
	return NULL;
}

