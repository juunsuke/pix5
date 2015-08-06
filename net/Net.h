#pragma once


namespace Net
{



uint32_t resolve(const Str& addr);
// Synchronously resolve a domain name to an IP address


NetPacket *new_packet();
// Allocate/recycle a packet

void recycle_packet(NetPacket *pak);
// Recycle an unused packet


}
