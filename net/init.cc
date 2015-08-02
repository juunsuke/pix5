#include "pix5net.h"


static void net_init()
{
}

static void net_shutdown()
{
}


static void net_reg() __attribute__((constructor()));
static void net_reg()
{
	pix_reg_module("PIX Network", net_init, net_shutdown);
}




