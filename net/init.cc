#include "pix5net.h"


namespace Net
{
	void init();
	void done();
}



static void net_init()
{
	Net::init();
}

static void net_shutdown()
{
	Net::done();
}


static void net_reg() __attribute__((constructor()));
static void net_reg()
{
	pix_reg_module("PIX Network", net_init, net_shutdown);
}




