#pragma once


#define DEFAULT_UDP_TIMEOUT		200


class UdpEndPoint: public EndPoint
{
	UdpSocket _sok;
	// The UDP socket

	SockAddr _peer;
	// Peer address/port

	UdpOutQueue _out;
	// The out queue

	UdpInQueue _in;
	// The in queue


	void send(NetPacket *pak);
	// Send a packet

	void send_ack(uint32_t serial);
	// Send an ACK packet for the given serial

	NetPacket *recv(SockAddr& peer);
	// Receive a packet
	// Returns NULL if there are no packets

	void handle_listening();
	void handle_connecting();
	void handle_connected();
	
	void do_send_message(NetMessage *msg);
	NetMessage *do_recv_message();

public:

	UdpEndPoint();
	~UdpEndPoint();


	void connect(const SockAddr& peer);
	// Initiate a connection to the given peer

	void listen(const SockAddr& bind_addr);
	// Listen to the given address, waiting for an incoming connection


	void handle();
	// Perform maintenance on the connection
};

