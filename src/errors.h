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


// Graphics errors
PIXERR( 0x10000001, SetMode,			"Error setting video mode" )
PIXERR( 0x10000002, TextureSize,		"Invalid texture size" )
PIXERR( 0x10000003, OpenGL,				"OpenGL error" )
PIXERR( 0x10000004, LoadTexture,		"Error loading a texture" )
PIXERR( 0x10000005, VertexDef,			"Invalid vertex definition" )
PIXERR( 0x10000006, ShaderCompile,		"Shader compilation error" )
PIXERR( 0x10000007, ShaderLink,			"Shader linking error" )
PIXERR( 0x10000008, NoUniform,			"No uniform found with that name" )
PIXERR( 0x10000009, BadUniformType,		"Uniform type mismatch" )
PIXERR( 0x1000000A, BadVertexComp,		"Invalid vertex component" )
PIXERR( 0x1000000B, BadVertexDef,		"Invalid vertex definition" )
PIXERR( 0x1000000C, NoAttribute,		"Cannot find the specified attribute" )
PIXERR( 0x1000000D, BadStreamIndex,		"Invalid stream index" )
PIXERR( 0x1000000E, FreeType,			"FreeType error" )
PIXERR( 0x1000000F, BadAnimSet,			"Invalid animation set" )


// Audio errors
PIXERR( 0x20000001, AudioStarted,		"Audio sub-system already started" )
PIXERR( 0x20000002, AudioStart,			"Error starting audio" )
PIXERR( 0x20000003, LoadVorbis,			"Error loading Ogg Vorbis file" )
PIXERR( 0x20000004, BadTrack,			"Invalid track index" )


// GUI errors
PIXERR( 0x30000001, HasParent,			"The element has a parent" )
PIXERR( 0x30000002, NoParent,			"The element has no parent" )
PIXERR( 0x30000003, NoChild,			"The child element was not found" )


// Map errors
PIXERR( 0x40000001, BadTileCoord,		"Invalid tile coordinates" )
PIXERR( 0x40000002, BadLayerType,		"Invalid layer type" )
PIXERR( 0x40000003, BadTileIndex,		"Invalid tile index" )


// Input errors
PIXERR( 0x50000001, BadJoyID,			"Invalid joystick ID" )
PIXERR( 0x50000002, BadJoyAxis,			"Invalid joystick axis index" )
PIXERR( 0x50000002, BadJoyButton,		"Invalid joystick button index" )
PIXERR( 0x50000002, BadJoyHat,			"Invalid joystick hat index" )


// LUA errors
PIXERR( 0x60000001, Lua,				"Lua error" )


// I/O errors
PIXERR( 0x70000001, BadMountPoint,		"Invalid mount point" )
PIXERR( 0x70000002, BadPath,			"Invalid path" )
PIXERR( 0x70000003, OpenFile,			"Error opening a file" )
PIXERR( 0x70000004, ClosedFile,			"The file is closed" )
PIXERR( 0x70000005, ReadFile,			"Error reading file" )






