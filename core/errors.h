#pragma once

//
// Error defintions
//


#define PIXERR(code, name, desc) \
	namespace E { \
		const uint32_t E ## name = code; \
		inline static void name(const char *fmt = NULL, ...) { \
			Str msg; \
			if(fmt) { va_list vl; va_start(vl, fmt); msg = Str::buildv(fmt, vl); } \
			throw Error(code, "E::E" #name, desc, msg); \
		} \
	}


// General errors
PIXERR(	0x00000001,	Unknown,			"Unknown error" )
PIXERR( 0x00000002,	Assert,				"Debug assertion failed" )
PIXERR( 0x00000003, SDL,				"SDL error" )
PIXERR( 0x00000004, OutOfBounds,		"Value out of bounds" )
PIXERR( 0x00000006, LoadFile,			"Error opening/reading a file" )
PIXERR( 0x00000007, EmptyFIFO,			"FIFO list is empty" )
PIXERR( 0x00000008, DoubleCache,		"Double cache entry" )
PIXERR( 0x00000009, Lua,				"Lua error" )
PIXERR( 0x0000000A, LuaAPI,				"Lua API error" )
PIXERR( 0x0000000B, LuaRuntime,			"Lua runtime error" )
PIXERR( 0x0000000C, Cache,				"Cache error" )
PIXERR(	0x0000000D, ZLib,				"zlib error" )


// Graphics errors
PIXERR( 0x10000001, SetMode,			"Error setting video mode" )
PIXERR( 0x10000002, TextureSize,		"Invalid texture size" )
PIXERR( 0x10000003, OpenGL,				"OpenGL error" )
PIXERR( 0x10000004, LoadTexture,		"Error loading a texture" )
PIXERR( 0x10000005, FreeType,			"FreeType error" )
PIXERR( 0x10000005, Renderer,			"Renderer error" )


// IO errors
PIXERR( 0x20000001, BadMountPoint,		"Invalid mount point" )
PIXERR( 0x20000002, OpenFile,			"Error opening a file" )
PIXERR( 0x20000003, FileIO,				"File I/O error" )
PIXERR( 0x20000004, DirIO,				"Directory error" )


// Input errors
PIXERR( 0x30000001, BadJoyID,			"Invalid joystick ID" )
PIXERR( 0x30000002, BadJoyAxis,			"Invalid joystick axis index" )
PIXERR( 0x30000002, BadJoyButton,		"Invalid joystick button index" )
PIXERR( 0x30000002, BadJoyHat,			"Invalid joystick hat index" )


// Network errors
PIXERR( 0x40000001, NetResolve,			"Could not resolve host name" )
PIXERR( 0x40000002, Socket,				"Socket error" )
PIXERR( 0x40000003, EndPoint,			"EndPoint error" )
PIXERR( 0x40000004, NetMessage,			"NetMessage error" )


// Audio errors
PIXERR( 0x50000001, LoadVorbis,			"Error loading an Ogg Vorbis file" )


// Map errors
PIXERR( 0x60000001, TileMap,			"TileMap error" )


// Game errors
PIXERR( 0x70000001, Scene,				"Scene error" )







