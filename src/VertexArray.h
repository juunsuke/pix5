#pragma once


class VertexArray
{
	List<VertexBuffer*> _vb;
	// All the vertex buffers, each one being a stream

	uint32_t _vao;
	// OpenGL VAO

	int _size;
	// Max locked size

	void create_gl(Shader *shad);
	// Create the OpenGL VAO and the associated VBOs

public:

	VertexArray();
	~VertexArray();

	int add_stream(const VertexDef& def, int stride, VertexBufferUsage::Type usage = VertexBufferUsage::Dynamic);
	// Add a new stream to the vertex array
	// Returns the new stream's index
	// Changes to the list of streams will be reflected with the next bind
	// This will cause the OpenGL VAO to be re-created if it already exists

	void delete_gl();
	// Delete the OpenGL VAO if it exists, along with all the attached VBOs

	void clear_streams();
	// Clear all the streams
	// This will also delete the OpenGL VAO

	void *lock(int stream, int first, int count);
	// Lock a stream's data
	// See VertexBuffer::lock() for more info

	void bind(Shader *shad);
	// Bind the OpenGL VAO
	// A mode must be set
	// The shader is required to lookup the attribute names and retrieve their indices
	// The shader need not be bound, but must be compiled

	static void unbind();
	// Unbind the VAO
};

