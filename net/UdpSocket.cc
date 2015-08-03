#include "pix5net.h"
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


UdpSocket::UdpSocket()
{
	// Create a socket
	_s = socket(AF_INET, SOCK_DGRAM, 0);
	if(_s==-1)
		E::Socket("socket() failed");

	// Make it non-blocking
	if(fcntl(_s, F_SETFL, fcntl(_s, F_GETFL, 0) | O_NONBLOCK)==-1)
		E::Socket("fcntl() failed");
}

UdpSocket::~UdpSocket()
{
	close();
}

void UdpSocket::close()
{
	// Close the socket
	if(_s!=-1)
	{
		::close(_s);
		_s = -1;
	}
}

void UdpSocket::bind(const SockAddr& sa)
{
	// Bind the socket
	if(::bind(_s, sa, sizeof(sockaddr_in)))
		E::Socket("bind() failed: %s", strerror(errno));

	Log::log("{%i} Socket bound to %s", _s, sa.format_ip_port().ptr());
}

bool UdpSocket::send(const SockAddr& sa, const void *ptr, int size)
{
	// Send a packet
	printf("Sending %i bytes\n", size);
	return sendto(_s, ptr, size, 0, sa, sizeof(sockaddr_in))!=-1;
}

int UdpSocket::recv(SockAddr& sa, void *ptr)
{
	// Try to receive a packet
	socklen_t len = sizeof(sockaddr_in);
	return Math::max(0, recvfrom(_s, ptr, 512, 0, (sockaddr*)&sa.sin, &len));
}

