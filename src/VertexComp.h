#pragma once


class VertexComp
{
	void copy(const VertexComp& o)
	{
		name = o.name;
		type = o.type;
		norm = o.norm;
		size = o.size;
		offset = o.offset;
	}

public:

	Str name;
	// Name of the generic attribute, to be matched with a shader attribute

	int type;
	// OpenGL data type

	bool norm;
	// Wether the data should be normalized

	int size;
	// Number of components, between 1 and 4

	int offset;
	// Offset, in bytes, into the vertex block that this component is found



	VertexComp()
	{
		type = 0;
		norm = false;
		size = 0;
		offset = 0;
	}

	VertexComp(const Str& name, int type, bool norm, int size, int offset)
	{
		this->name = name;
		this->type = type;
		this->norm = norm;
		this->size = size;
		this->offset = offset;
	}

	VertexComp(const VertexComp& o)
	{
		copy(o);
	}

	VertexComp& operator=(const VertexComp& o)
	{
		copy(o);
		return *this;
	}

	~VertexComp()
	{
	}

	int get_valid_size() const;
	// This validates that the 'type' and 'size' fields are valid, and return the
	// total size, in bytes, used by one entry of the component
	// That is, the size of 'type' multiplied by 'size'


	//
	// Static functions to create the most common data types
	//

	static VertexComp float1(const Str& name, int offset);
	static VertexComp float2(const Str& name, int offset);
	static VertexComp float3(const Str& name, int offset);
	static VertexComp float4(const Str& name, int offset);

	static VertexComp byte1(const Str& name, bool norm, int offset);
	static VertexComp byte2(const Str& name, bool norm, int offset);
	static VertexComp byte3(const Str& name, bool norm, int offset);
	static VertexComp byte4(const Str& name, bool norm, int offset);

	static VertexComp ubyte1(const Str& name, bool norm, int offset);
	static VertexComp ubyte2(const Str& name, bool norm, int offset);
	static VertexComp ubyte3(const Str& name, bool norm, int offset);
	static VertexComp ubyte4(const Str& name, bool norm, int offset);

	static VertexComp short1(const Str& name, bool norm, int offset);
	static VertexComp short2(const Str& name, bool norm, int offset);
	static VertexComp short3(const Str& name, bool norm, int offset);
	static VertexComp short4(const Str& name, bool norm, int offset);

	static VertexComp ushort1(const Str& name, bool norm, int offset);
	static VertexComp ushort2(const Str& name, bool norm, int offset);
	static VertexComp ushort3(const Str& name, bool norm, int offset);
	static VertexComp ushort4(const Str& name, bool norm, int offset);

	static VertexComp int1(const Str& name, bool norm, int offset);
	static VertexComp int2(const Str& name, bool norm, int offset);
	static VertexComp int3(const Str& name, bool norm, int offset);
	static VertexComp int4(const Str& name, bool norm, int offset);

	static VertexComp uint1(const Str& name, bool norm, int offset);
	static VertexComp uint2(const Str& name, bool norm, int offset);
	static VertexComp uint3(const Str& name, bool norm, int offset);
	static VertexComp uint4(const Str& name, bool norm, int offset);

};

