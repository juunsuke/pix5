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


// Graphics error
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


