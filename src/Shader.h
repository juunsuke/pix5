#pragma once


class ShaderUniformAttrib
{
public:
	
	int index;
	// Uniform/attribute index

	Str name;
	// Uniform/attribute name

	int type;
	// Uniform/attribute type (OpenGL enum)

	int size;
	// Number of elements if an array
};

class Shader
{
	Str _name;
	// Shader name, for logging purposes

	char *_vs_src, *_fs_src;
	// Vertex and Fragment Shader sources

	uint32_t _prg;
	// OpenGL program object

	List<ShaderUniformAttrib*> _uniforms;
	List<ShaderUniformAttrib*> _attribs;
	// List of uniforms and attributes, filled in after compilation


	Shader(const Str& name);
	// Used internally, use the static functions to create shaders

	uint32_t compile(int type, const char *src);
	// Compile a single shader

	ShaderUniformAttrib *get_uniform(const Str& name, int type);
	ShaderUniformAttrib *get_attrib(const Str& name);
	// Find the uniform/attrib with the given name

public:

	~Shader();
	
	static Shader *create_from_source(const Str& name, const char *vs_src, int vs_len, const char *fs_src, int fs_len);
	// Create a shader from raw data
	// 'vs_src' and 'fs_src' are strings containing the whole source code for each shader
	// The shader instance must always have access to the original source, in case
	// it needs to me recompiled (like after a mode switch or context loss of some sort)
	// If 'copy' is true, the shader instance will make its own copy of both sources
	// Otherwise, it will simply point to the given buffers, which shouldn't be freed
	// as long as the shader instance is still in use

	static Shader *load(const Str& name, const Str& vs_file, const Str& fs_file);
	// Create a shader from source files
	// 'vs_file' and 'fs_file' are names of files containing shader sources


	void compile();
	// Compile the shader
	// Upon creation, shaders are not immediatly compiled, as compilation requires
	// a valid OpenGL context
	// As such, compilation can only be performed once a mode is set
	// You can either call compile() directly to control when the compilation occurs,
	// or simply ignore it, as bind() will compile if needed
	// Shaders must be recompiled after a mode chage, as a new OpenGL context is
	// created each time
	// If the shader is already compiled, this function does nothing

	void bind();
	// Bind the shader with OpenGL
	// If not compiled yet, it will be compiled at this time

	static void unbind();
	// Unbind any shader

	int get_attribute_index(const Str& name);
	// Find the index of the given attribute
	// Returns -1 if the attribute was not found

	void set_uniform(const Str& name, const Matrix& mat);
	// Set a single matrix uniform

	void set_uniform(const Str& name, int index);
	// Set a single integer/texture uniform

};



