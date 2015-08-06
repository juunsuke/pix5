#pragma once


class EndPoint
{
protected:

	EndPointState::Type _state;
	// Current state

	Str _error;
	// Error message when the EndPoint is in error state


	virtual void do_send_message(NetMessage *msg) = 0;
	virtual NetMessage *do_recv_message() = 0;

public:

	EndPoint();
	virtual ~EndPoint();


	EndPointState::Type state() { return _state; }
	Str error() { return _error; }
	// Get the state and error

	void send_message(NetMessage *msg);
	// Send a message to the connected peer
	// Will throw an error if the peer state is not Connected
	// The EndPoint takes ownership of 'msg', which must
	// be considered invalid when this function returns
	// If an error occurs, 'msg' will still be deleted

	NetMessage *recv_message();
	// Receive a message from the connected peer
	// Will throw an error if the peer state is not Connected
	// If no message is pending, NULL will be returned
	// The returned message is caller-owned, and must be deleted once handled
};

