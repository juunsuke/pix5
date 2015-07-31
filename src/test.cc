#include "pix5.h"


int main(int argc, char **argv)
{
	LuaGame *lg = new LuaGame();
	lg->run("test.lua");
	delete lg;

	return 0;
}


