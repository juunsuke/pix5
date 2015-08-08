#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include <pthread.h>
#include <sys/time.h>


#ifdef DBG
	#include <typeinfo>
	#include <cxxabi.h>
#endif

#ifdef DBG
#ifndef NO_OVERLOAD_NEW_DELETE
	void *operator new(size_t size, const char *file, int line);
	#define new new(__FILE__, __LINE__)

	void new_delete(void *ptr);
#endif
#endif


#include "Str.h"
#include "Error.h"
#include "errors.h"


#define MAX_PIX_MODULES 64

void pix_reg_module(const char *name, void (*init)(), void (*shutdown)());
// Register an internal PIX module to be initialized/shutdown by pix_init() and pix_shutdown()
// Shouldn't normally be called unless you write a PIX extension of some sort


void pix_init(const Str& app_name);
// Initialize PIX
// This function should be called before doing anything else

void pix_shutdown();
// Shutdown PIX
// This will free any used resources and bring the system back to the
// state it was before calling PIX::init()

bool was_init();
// Returns wether PIX was initialized

Str get_app_name();
// Retrieve the registered app name

int64_t get_ms();
// Get an ever-increasing time value in milliseconds
// Usually the time since the OS booted, but not guaranteed to be so

#ifdef DBG
	#define ASSERT(x, m) if(!(x)) E::Assert("%s: %i: %s", __FILE__, __LINE__, m);
	#define ASSERT_INIT(m) if(!PIX::was_init()) E::Assert("%s: %i: PIX was not initialized: %s", __FILE__, __LINE__, m);
#else
	#define ASSERT(x, m)
	#define ASSERT_INIT(m)
#endif

#ifdef DBG
	inline static Str demangle(const char *name)
	{
		// Demangle a type name
		int status = 0;
		char *ptr = abi::__cxa_demangle(name, NULL, NULL, &status);
		Str s(ptr);
		free(ptr);

		return s;
	}
#endif

#include "Log.h"
#include "Buffer.h"
#include "List.h"
#include "SortedList.h"
#include "FIFO.h"
#include "Math.h"
#include "RNG.h"

#include "Mutex.h"
#include "Condition.h"
#include "Barrier.h"
#include "RWLock.h"
#include "Thread.h"
#include "Job.h"
#include "JobThread.h"
#include "ThreadPool.h"

#include "FilePath.h"
#include "FileAccess.h"
#include "File.h"
#include "FileUnix.h"
#include "FileEntry.h"
#include "MountEntry.h"
#include "IOSource.h"
#include "IOSourceUnix.h"
#include "IO.h"

// #include "SoundClip.h"
// #include "Audio.h"
 
 #include "Cache.h"


#include "CoreEventHandler.h"
//#include "Game.h"
//
//#include "luaP.h"
//#include "LuaAPI.h"
//#include "Lua.h"
//#include "LuaGame.h"




