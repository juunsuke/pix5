#include "pix5core.h"

namespace Log {


void plog(const char *code, const char *fmt, va_list vl)
{
	printf("[%s] ", code);
	vprintf(fmt, vl);
	printf("\n");
}

void plog(const char *code, const Str& str)
{
	printf("[%s] %s\n", code, str.ptr());
}

void log(const char *fmt, ...)
{
	// Add a regular log entry
	va_list vl;
	va_start(vl, fmt);
	plog("   ", fmt, vl);
}

void error(const Error &err)
{
	// Log an error
	plog("ERR", err.format());
}


#ifdef DBG
void debug(const char *fmt, ...)
{
	// Debug log
	va_list vl;
	va_start(vl, fmt);
	plog("DBG", fmt, vl);
}
#endif





}

