#include "pix5net.h"


TcpSocket::TcpSocket()
{
	_connected = false;
}

TcpSocket::~TcpSocket()
{
}

void TcpSocket::create()
{
	ASSERT(_s==INVALID_SOCKET, "TcpSocket::create(): Socket already created")

	// Create a socket
	_s = socket(AF_INET, SOCK_STREAM, 0);
	if(_s==INVALID_SOCKET)
		E::SocketCreate("Error creating a TCP socket");
}

void TcpSocket::connect(const SockAddr& sa)
{
	if(_s==INVALID_SOCKET)
		E::SocketConnect("Trying to connect an empty socket");

	if(_connected)
		E::SocketConnect("The socket is already connected");

	// Try to connect
	if(::connect(_s, sa, sizeof(sockaddr)))
		E::SocketConnect("Error connecting the socket");

	#ifdef DBG
	Log::debug("TCP connection established to %s", sa.format_ip_port().ptr());
	#endif
}


