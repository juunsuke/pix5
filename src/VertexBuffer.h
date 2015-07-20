#pragma once


namespace VertexBufferUsage
{
	enum Type
	{
		Static,
		Dynamic,
		Stream,
	};
}


class VertexBuffer
{
	VertexDef _def;
	// The definition for this buffer

	int _stride;
	// The buffer stride

	VertexBufferUsage::Type _usage;
	// Buffer usage

	int _size;
	// Number of elements in the buffer

	void *_data;
	// Actual data buffer

	bool _dirty;
	// Dirty flag;

	uint32_t _vbo[2];
	// OpenGL VBO objects

	bool _double_buf;
	// Wether to use double VBOs

	int _cur_vbo;
	// Next VBO to use when double-buffering


	void create_gl(int i);
	void create_gl();
	// Create the OpenGL VBO, bind it, and upload data

public:

	VertexBuffer(const VertexDef& def, int stride, VertexBufferUsage::Type usage = VertexBufferUsage::Dynamic, bool double_buf = false);
	// The definition will be validated and a copy will be made internally
	// If the definition isn't valid, an error will be thrown
	// The stride must be big enough to contain the provided definition
	// If 'double' is true, the VB will be double-buffered, so 2 OpenGL VBOs will be created

	~VertexBuffer();

	void delete_gl();
	// Delete the OpenGL VBO if it exists

	void clear_data();
	// Clear the buffer data, while keeping the definition

	int stride() const { return _stride; }
	// Return the stride

	int size() const { return _size; }
	// Return the size, in elements, of the buffer

	VertexBufferUsage::Type usage() const { return _usage; }
	// Return the current usage type
	
	bool is_dirty() const { return _dirty; }
	// Return the dirty flag
	
	VertexDef get_def() const { return _def; }
	// Return a copy of the vertex definition

	void set_def(const VertexDef& def, int stride);
	// Set a new definition and stride
	// This will also clear the data

	void set_usage(VertexBufferUsage::Type usage);
	// Change the usage type
	// This will cause the VBO to be destroyed and re-created, if applicable

	void resize(int size);
	// Resize the buffer to be able to hold 'size' vertices
	// Previous data will be preserved, but if enlarged the new area will not be cleared
	// This will cause the VBO to be recreated

	void *lock(int first, int count);
	// Retrive a pointer to an area of the buffer
	// 'first' is the vertex index that the returned pointer will point to
	// 'count' is the number of vertices guaranteed to be available in the buffer
	// If the buffer is not large enough, it will be enlarged
	// If count is less or equal to 0, NULL will be returned
	// The returned pointer is valid until a new lock() or resize() call is made
	// The returned pointer is also invalidated by clearing the data, with clear_data() or set_def()
	// If the buffer is enlarged, the VBO will be recreated

	void bind();
	// Bind the OpenGL VBO
	// A mode must be set
	// The VBO will be created if needed, and data will be (re)uploaded if it changed

	static void unbind();
	// Unbind any OpenGL VBO

	void set_attribs(Shader *shad);
	// Setup the vertex attrib pointers, based on the VB definition, and enable them
	// The shader is required to lookup the attribute names and retrieve their indices
	// The shader need not be bound, but must be compiled

	static void disable_attribs();
	// Disable all vertex attribs
};


