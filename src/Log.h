#pragma once


namespace Log
{



void plog(const char *code, const char *fmt, va_list vl);
void plog(const char *code, const Str& str);
// Raw/custom log output

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
