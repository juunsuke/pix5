#include "pix5net.h"


UdpEndPoint::UdpEndPoint()
{
}

UdpEndPoint::~UdpEndPoint()
{
}

void UdpEndPoint::connect(const SockAddr& peer)
{
	// Validate
	if(_state!=EndPointState::New)
		E::EndPoint("UdpEndPoint::connect(): Invalid EndPoint status");

	// Start connecting
	_state = EndPointState::Connecting;
	_peer = peer;

	// Create the initial packet
	NetPacket *pak = Net::new_packet();

	// Add it to the queue, which will give it serial number 0 and timestamp it
	_out.add(pak);

	// Add the hello message
	strcpy((char*)pak->_data.data, "HELLO_PIX");
	pak->_size += 9;

	send(pak);
}
	
void UdpEndPoint::listen(const SockAddr& bind_addr)
{
	// Validate
	if(_state!=EndPointState::New)
		E::EndPoint("UdpEndPoint::listen(): Invalid EndPoint status");

	// Start listening
	_state = EndPointState::Listening;

	// Bind
	try
	{
		_sok.bind(bind_addr);
	}
	catch(Error e)
	{
		// Some sort of failure, set the state to error
		_error = e.format();
		_state = EndPointState::Error;
	}
}

void UdpEndPoint::send(NetPacket *pak)
{
	// Try to send a packet
	try
	{
		//printf("Sending a %i bytes packet\n", pak->_size);
		_sok.send(_peer, pak->_data.raw, pak->_size);
	}
	catch(Error e)
	{
		// Some sort of failure, set the state to error
		_error = e.format();
		_state = EndPointState::Error;
	}
}
	
void UdpEndPoint::send_ack(uint32_t serial)
{
	// Try to send an ACK packet
	try
	{
		//printf("Sending ACK for serial %i\n", serial);
		_sok.send(_peer, &serial, NET_PACKET_HEADER_SIZE);
	}
	catch(Error e)
	{
		// Some sort of failure, set the state to error
		_error = e.format();
		_state = EndPointState::Error;
	}
}
	
NetPacket *UdpEndPoint::recv(SockAddr& peer)
{
	// Check for incoming packets
	for(;;)
	{
		if(_state==EndPointState::Error)
			return NULL;

		try
		{
			// Try to receive a raw packet
			char buf[512];
			int r = _sok.recv(_peer, buf);
		
			if(r==0)
				// Nothing
				return NULL;
		
			// A packet is in
			if(r==NET_PACKET_HEADER_SIZE)
			{
				// An ACK packet
				uint32_t serial = *(uint32_t*)buf;;

				//printf("Received ACK for packet with serial %i\n", serial);

				// Remove it from the out queue
				_out.remove(serial);

				continue;
			}
			else
			{
				// A data packet
				NetPacket *pak = Net::new_packet();
				memcpy(pak->_data.raw, buf, r);
				pak->_size = r;

				//printf("Received a %i bytes packet with serial %i\n", pak->_size, pak->_data.serial);

				// Send an ACK
				send_ack(pak->_data.serial);

				return pak;
			}
		}
		catch(Error e)
		{
			// Some sort of failure, set the state to error
			_error = e.format();
			_state = EndPointState::Error;
			return NULL;
		}
	}
}

void UdpEndPoint::handle_listening()
{
	// Check for an incoming packet
	NetPacket *pak = recv(_peer);
	if(!pak)
		return;

	// The only valid packet we can receive is the HELLO_PIX message
	if(pak->_size!=(NET_PACKET_HEADER_SIZE+9) || strncmp((char*)pak->_data.data, "HELLO_PIX", 9))
	{
		// Not a valid packet
		Log::log("Invalid greeting received from %s, ignoring", _peer.format_ip().ptr());
		Net::recycle_packet(pak);
		return;
	}

	Log::log("Peer connected from %s", _peer.format_ip_port().ptr());
		
	Net::recycle_packet(pak);

	// Mark the EndPoint as connected
	_state = EndPointState::Connected;

	// Send a greeting
	pak = Net::new_packet();

	// Add it to the queue, which will give it serial number 0 and timestamp it
	_out.add(pak);

	// Add the welcome message
	strcpy((char*)pak->_data.data, "WELCOME_PIX");
	pak->_size += 11;

	send(pak);
}

void UdpEndPoint::handle_connecting()
{
	// Check for an incoming packet
	NetPacket *pak = recv(_peer);
	if(!pak)
		return;

	// The only valid packet we can receive is the WELCOME_PIX message
	if(pak->_size!=(NET_PACKET_HEADER_SIZE+11) || strncmp((char*)pak->_data.data, "WELCOME_PIX", 11))
	{
		// Not a valid packet
		Log::log("Invalid greeting received from %s, aborting", _peer.format_ip_port().ptr());
		Net::recycle_packet(pak);

		_error = "Invalid greeting received from server";
		_state = EndPointState::Error;
		return;
	}

	Log::log("Connected to %s", _peer.format_ip_port().ptr());

	Net::recycle_packet(pak);

	// Mark as connected
	_state = EndPointState::Connected;
}

void UdpEndPoint::handle_connected()
{
	// Check for incoming packets
	while(NetPacket *pak = recv(_peer))
		// Add it to the in queue
		_in.add(pak);
}

void UdpEndPoint::handle()
{
	// Handle states
	switch(_state)
	{
		case EndPointState::Listening:
			handle_listening();
			break;

		case EndPointState::Connecting:
			handle_connecting();
			break;

		case EndPointState::Connected:
			handle_connected();
			break;

		case EndPointState::New:
		case EndPointState::Error:
			return;
	}

	// Check for expired packets
	int64_t now = get_ms();
	int64_t time = now-DEFAULT_UDP_TIMEOUT;

	for(NetPacket *pak = NULL; (pak = _out.get_expired(time, pak));)
	{
		// Set the new timestamp
		pak->_ts = now;

		// Resend it
		send(pak);
	}
}

void UdpEndPoint::do_send_message(NetMessage *msg)
{
	// Packetise the message and send the packets

	// Get the number of required packets
	int size = msg->size();
	int num = (msg->size()+NET_PACKET_DATA_SIZE-1)/NET_PACKET_DATA_SIZE;
	int pos = 0;

	if(num>255)
		E::EndPoint("UdoEndPoint::do_send_message(): Maximum UDP message size is 126,480 bytes");

	for(int c = 0; c<num; c++)
	{
		// Create a packet
		NetPacket *pak = Net::new_packet();
		pak->_data.flags = (num&0xFF);

		// Copy the data
		int sz = Math::min(size-pos, NET_PACKET_DATA_SIZE);
		memcpy(pak->_data.data, msg->ptr()+pos, sz);
		pos += sz;

		pak->_size += sz;

		// Send it
		_out.add(pak);
		send(pak);
	}

	// Don't need the message anymore
	delete msg;
}

NetMessage *UdpEndPoint::do_recv_message()
{
	// Check if there's a full message in the queue
	if(!_in.has_message())
		return NULL;

	// There's a message, get it
	NetMessage *msg = new NetMessage();

	// Grab the first packet
	NetPacket *pak = _in.get();

	// Check the number of packets in the message
	int num = pak->_data.flags & 0xFF;

	// Add all those packets to the message
	while(num)
	{
		// Add the raw data
		msg->add_raw(pak->_data.data, pak->_size-NET_PACKET_HEADER_SIZE);

		Net::recycle_packet(pak);

		// Check if we're done
		num--;
		if(!num)
			break;

		// Get the next one
		pak = _in.get();
	}

	// Reset the position
	msg->reset_pos();

	// Done
	return msg;
}



