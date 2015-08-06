#include "pix5net.h"

#ifdef WIN32
#else
	#include <netdb.h>
#endif


namespace Net {


NetPacket *_free_packet = NULL;
// Pointer for the first free packet



void init()
{
	_free_packet = NULL;
}

void done()
{
	// Delete all recycled, unused packets
	while(_free_packet)
		delete new_packet();
}



uint32_t resolve(const Str& addr)
{
	// Try to resolve
	hostent *he = gethostbyname(addr);

	if(!he)
		E::NetResolve("'%s' could not be resolved", addr.ptr());

	// Return the address
	uint32_t ip = ntohl(*(uint32_t*)(he->h_addr_list[0]));
	Log::log("Resolved '%s' to %s", addr.ptr(), SockAddr(ip).format_ip().ptr());

	return ip;
}

NetPacket *new_packet()
{
	// Allocate/recycle a packet
	NetPacket *pak;

	// If there is a free one, recycle it
	if(_free_packet)
	{
		pak = _free_packet;
		_free_packet = pak->_next;
	}
	else
		pak = new NetPacket();

	pak->_size = NET_PACKET_HEADER_SIZE;
	pak->_prev = NULL;
	pak->_next = NULL;

	pak->_data.flags = 0;

	return pak;
}

void recycle_packet(NetPacket *pak)
{
	// Add the packet to the free list
	pak->_next = _free_packet;
	_free_packet = pak;
}



}


