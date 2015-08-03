#pragma once


class UdpSocket
{
	int _s;
	// The actual socket

public:

	UdpSocket();
	~UdpSocket();

	void close();
	// Close the socket

	void bind(const SockAddr& sa);
	// Bind the socket to the given address

	bool send(const SockAddr& sa, const void *ptr, int size);
	// Send a UDP packet to the given destination
	// Returns false if the call immediatly fails, or true if it succeeds

	int recv(SockAddr& sa, void *ptr);
	// Try to receive a UDP packet
	// Returns the number of bytes received, or 0 if there was no packet
	// If there was a packet, it will be stored at the address pointed to
	// by 'ptr', which must be at least 512 bytes, and the sender's address
	// and port will be stored in 'sa'
};

