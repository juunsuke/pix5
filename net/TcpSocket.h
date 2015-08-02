#pragma once


class TcpSocket: public NetSocket
{
	bool _connected;
	// Wether the socket is connected

	NetBuffer _in, _out;
	// In and out buffers

public:

	TcpSocket();
	~TcpSocket();


	void create();
	// Create the socket

	void connect(const SockAddr& sa);
	// Connect to the given address
};

