#pragma once

#include "../core/pix5core.h"


#ifdef WIN32

#else
	
	// Linux
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>

	typedef int SOCKET;
	#define INVALID_SOCKET -1

#endif



#include "SockAddr.h"
#include "NetBuffer.h"
#include "NetSocket.h"
#include "TcpSocket.h"

#include "Net.h"

