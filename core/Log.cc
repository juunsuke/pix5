#include "pix5core.h"

namespace Log {

Mutex _mtx;


void plog(const char *code, const char *fmt, va_list vl)
{
	_mtx.lock();

	printf("[%s] ", code);
	vprintf(fmt, vl);
	printf("\n");

	_mtx.unlock();
}

void plog(const char *code, const Str& str)
{
	_mtx.lock();

	printf("[%s] %s\n", code, str.ptr());

	_mtx.unlock();
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

