#include "pix5net.h"


EndPoint::EndPoint()
{
	_state = EndPointState::New;
}

EndPoint::~EndPoint()
{
}

void EndPoint::send_message(NetMessage *msg)
{
	try
	{
		// Make sure the state is connected
		if(_state!=EndPointState::Connected)
			E::EndPoint("EndPoint::send_message(): Not connected to a peer");

		// Let the descending class handle the sending
		do_send_message(msg);
	}
	catch(Error)
	{
		delete msg;
		throw;
	}
}

NetMessage *EndPoint::recv_message()
{
	// Make sure the state is connected
	if(_state!=EndPointState::Connected)
		E::EndPoint("EndPoint::recv_message(): Not connected to a peer");

	// Let the descending class handle the receiving
	return do_recv_message();
}

