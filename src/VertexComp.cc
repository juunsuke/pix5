#include "pix5.h"
#include <GL/glew.h>

namespace PIX {

	
int VertexComp::get_valid_size() const
{
	// Validate the type, marking the base size
	int sz = 0;

	switch(type)
	{
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			sz = 1;
			break;

		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			sz = 2;
			break;

		case GL_INT:
		case GL_UNSIGNED_INT:
			sz = 4;
			break;

		case GL_HALF_FLOAT:
			sz = 2;
			break;

		case GL_FLOAT:
		case GL_FIXED:
			sz = 4;
			break;

		case GL_DOUBLE:
			sz = 8;
			break;

		default:
			E::BadVertexComp("VertexComp::get_valid_size(): Unrecognized/unsupported type: %i", type);
			return 0;
	}

	// Validate the size
	if(size<1 || size>4)
		E::BadVertexComp("VertexComp::get_valid_size(): Invalid component size: %i", size);

	// Return the total size
	return sz*size;
}


VertexComp VertexComp::float1(const Str& name, int offset)				{ return VertexComp(name, GL_FLOAT, false, 1, offset); }
VertexComp VertexComp::float2(const Str& name, int offset)				{ return VertexComp(name, GL_FLOAT, false, 2, offset); }
VertexComp VertexComp::float3(const Str& name, int offset)				{ return VertexComp(name, GL_FLOAT, false, 3, offset); }
VertexComp VertexComp::float4(const Str& name, int offset)				{ return VertexComp(name, GL_FLOAT, false, 4, offset); }

VertexComp VertexComp::byte1(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_BYTE, norm, 1, offset); }
VertexComp VertexComp::byte2(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_BYTE, norm, 2, offset); }
VertexComp VertexComp::byte3(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_BYTE, norm, 3, offset); }
VertexComp VertexComp::byte4(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_BYTE, norm, 4, offset); }

VertexComp VertexComp::ubyte1(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_BYTE, norm, 1, offset); }
VertexComp VertexComp::ubyte2(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_BYTE, norm, 2, offset); }
VertexComp VertexComp::ubyte3(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_BYTE, norm, 3, offset); }
VertexComp VertexComp::ubyte4(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_BYTE, norm, 4, offset); }

VertexComp VertexComp::short1(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_SHORT, norm, 1, offset); }
VertexComp VertexComp::short2(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_SHORT, norm, 2, offset); }
VertexComp VertexComp::short3(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_SHORT, norm, 3, offset); }
VertexComp VertexComp::short4(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_SHORT, norm, 4, offset); }

VertexComp VertexComp::ushort1(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_SHORT, norm, 1, offset); }
VertexComp VertexComp::ushort2(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_SHORT, norm, 2, offset); }
VertexComp VertexComp::ushort3(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_SHORT, norm, 3, offset); }
VertexComp VertexComp::ushort4(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_SHORT, norm, 4, offset); }

VertexComp VertexComp::int1(const Str& name, bool norm, int offset)		{ return VertexComp(name, GL_INT, norm, 1, offset); }
VertexComp VertexComp::int2(const Str& name, bool norm, int offset)		{ return VertexComp(name, GL_INT, norm, 2, offset); }
VertexComp VertexComp::int3(const Str& name, bool norm, int offset)		{ return VertexComp(name, GL_INT, norm, 3, offset); }
VertexComp VertexComp::int4(const Str& name, bool norm, int offset)		{ return VertexComp(name, GL_INT, norm, 4, offset); }

VertexComp VertexComp::uint1(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_INT, norm, 1, offset); }
VertexComp VertexComp::uint2(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_INT, norm, 2, offset); }
VertexComp VertexComp::uint3(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_INT, norm, 3, offset); }
VertexComp VertexComp::uint4(const Str& name, bool norm, int offset)	{ return VertexComp(name, GL_UNSIGNED_INT, norm, 4, offset); }



}
