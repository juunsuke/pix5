#include "pix5net.h"
#include <errno.h>
#include <fcntl.h>


UdpSocket::UdpSocket()
{
	// Create the socket
	_s = socket(AF_INET, SOCK_DGRAM, 0);
	if(_s==-1)
		E::Socket("socket() failed: %s", strerror(errno));

	// Make it non-blocking
	if(fcntl(_s, F_SETFL, fcntl(_s, F_GETFL, 0) | O_NONBLOCK)==-1)
		E::Socket("fcntl() failed: %s", strerror(errno));
}

UdpSocket::~UdpSocket()
{
}

int UdpSocket::send(const SockAddr& sa, const void *ptr, int size)
{
	// Send a packet
	int r = sendto(_s, ptr, size, 0, sa, sizeof(sockaddr_in));
	if(r<0)
		E::Socket("sendto() failed: %s", strerror(errno));

	return r;
}

void UdpSocket::bind(const SockAddr& sa)
{
	// Bind
	if(::bind(_s, sa, sizeof(sockaddr_in)))
		E::Socket("bind() failed: %s", strerror(errno));
}

int UdpSocket::recv(SockAddr& sa, void *ptr)
{
	// Try to receive data
	socklen_t len = sizeof(sockaddr_in);
	int r = recvfrom(_s, ptr, 512, 0, (sockaddr*)&sa.sin, &len);
	if(r<0)
	{
		if(errno==EWOULDBLOCK || errno==EAGAIN)
			return 0;

		E::Socket("recv() failed: %s", strerror(errno));
	}

	return r;
}



