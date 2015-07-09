#pragma once


namespace Log
{



void log(const char *fmt, ...);
// Add a standard log message notice to the log

void error(const Error &err);
// Log an error
// In debug mode, all errors are logged as they are created, before being thrown

#ifdef DBG
void debug(const char *fmt, ...);
#endif
// Add a debug log entry




}
