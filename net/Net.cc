#include "pix5net.h"

#ifdef WIN32
#else
	#include <netdb.h>
#endif


namespace Net {



void init()
{
}

void done()
{
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





}


