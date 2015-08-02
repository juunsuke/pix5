#pragma once


class NetSocket
{
protected:
	SOCKET _s;
	// The actual socket

public:

	NetSocket();
	~NetSocket();


	void bind(const SockAddr& sa);
	// Bind the socket to an address

};

