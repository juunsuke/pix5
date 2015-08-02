#include "pix5net.h"


NetSocket::NetSocket()
{
	_s = INVALID_SOCKET;
}

NetSocket::~NetSocket()
{
}

void NetSocket::bind(const SockAddr& sa)
{
	if(_s==INVALID_SOCKET)
		E::SocketBind("Trying to bind an empty socket");

	// Bind
	if(::bind(_s, sa, sizeof(sockaddr_in)))
		E::SocketBind("Binding failed");
}

