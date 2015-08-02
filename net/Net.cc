#include "pix5net.h"


namespace Net {




uint32_t resolve(const Str& addr)
{
	// Try to resolve
	hostent *he = gethostbyname(addr);

	if(!he)
		E::NetResolve("'%s' could not be resolved", addr.ptr());

	// Return the address
	uint32_t ip = ntohl(*(uint32_t*)(he->h_addr_list[0]));

	#ifdef DBG
	Log::debug("Resolved '%s' to %s", addr.ptr(), SockAddr(ip).format_ip().ptr());
	#endif

	return ip;
}





}


