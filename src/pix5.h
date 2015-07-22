#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include <pthread.h>
#include <sys/time.h>


struct SDL_Window;
struct _SDL_Joystick;
struct FT_FaceRec_;

#ifdef DBG
	#include <typeinfo>
	#include <cxxabi.h>
#endif

namespace PIX
{


#include "Str.h"
#include "Error.h"
#include "errors.h"


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

int load_file(const Str& fname, char **buf);
// Loads a file, allocates a buffer and sets its address into buf
// A terminal zero is added at the end
// Returns the number of bytes read, exclusing the terminal zero
// Returns -1 on error

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
#include "List.h"
#include "SortedList.h"
#include "FIFO.h"
#include "Math.h"

#include "Mutex.h"
#include "Condition.h"
#include "Barrier.h"
#include "RWLock.h"
#include "Thread.h"
#include "Job.h"
#include "JobThread.h"
#include "ThreadPool.h"

#include "Mouse.h"
#include "keys.h"
#include "Key.h"
#include "Keyboard.h"
#include "JoyAxis.h"
#include "JoyButton.h"
#include "JoyHat.h"
#include "Joystick.h"
#include "Input.h"

#include "VideoModeType.h"
#include "VideoMode.h"

#include "BlendMode.h"
#include "TextAlign.h"
#include "Color.h"
#include "Rect.h"
#include "Glyph.h"
#include "Font.h"
#include "Texture.h"

#include "Matrix.h"
#include "Camera.h"

#include "Shader.h"

#include "VertexComp.h"
#include "VertexDef.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

#include "Anim.h"
#include "Sprite.h"
#include "SpriteSet.h"

#include "Display.h"

#include "TexRenderer.h"

#include "SoundClip.h"
#include "Audio.h"

#include "Cache.h"

#include "GuiElement.h"
#include "Label.h"
#include "Button.h"

#include "GuiTheme.h"
#include "GUI.h"

#include "MapSprite.h"
#include "TMLayer.h"
#include "TMBase.h"
#include "TileMap.h"

#include "EventHandler.h"


}



#ifndef PIX_NO_USING
using namespace PIX;
#endif

