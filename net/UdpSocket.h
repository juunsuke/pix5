#pragma once


class UdpSocket
{
	int _s;
	// The system socket

public:

	UdpSocket();
	~UdpSocket();


	int send(const SockAddr& sa, const void *ptr, int size);
	// Send a packet to the given address

	int recv(SockAddr& sa, void *ptr);
	// Try to receive a packet
	// If no packet is received, 0 will be returned
	// If a packet is received, its size will be returned, the data will b e
	// written at the address pointed to by 'ptr' (which must be at least 512 bytes),
	// and the peer address will be stored into 'sa'

	void bind(const SockAddr& sa);
	// Bind the socket to the given address/port
};

