#include "pix5.h"
#include <GL/glew.h>

namespace PIX {


Shader::Shader(const Str& name)
{
	_name = name;
	_vs_src = NULL;
	_fs_src = NULL;
	_prg = 0;
}

Shader::~Shader()
{
	// Free the sources
	if(_vs_src)
		free(_vs_src);
	if(_fs_src)
		free(_fs_src);

	if(_prg)
		glDeleteProgram(_prg);

	_uniforms.clear_del();
	_attribs.clear_del();
}

Shader *Shader::create_from_source(const Str& name, const char *vs_src, int vs_len, const char *fs_src, int fs_len)
{
	// Create a shader
	Shader *shad = new Shader(name);

	try
	{
		// Copy the sources
		shad->_vs_src = (char*)malloc(vs_len+1);
		memcpy(shad->_vs_src, vs_src, vs_len);
		shad->_vs_src[vs_len] = 0;

		shad->_fs_src = (char*)malloc(fs_len+1);
		memcpy(shad->_fs_src, fs_src, fs_len);
		shad->_fs_src[fs_len] = 0;
	}
	catch(Error)
	{
		delete shad;
		throw;
	}

	return shad;
}

Shader *Shader::load(const Str& name, const Str& vs_file, const Str& fs_file)
{
	// Load both files
	char *vs, *fs;

	if(load_file(vs_file, &vs)==-1)
		E::LoadFile("Error loading vertex shader file: %s", vs_file.ptr());

	if(load_file(fs_file, &fs)==-1)
	{
		free(vs);
		E::LoadFile("Error loading fragment shader file: %s", fs_file.ptr());
	}

	try
	{
		// Create the shader
		Shader *shad = Shader::create_from_source(name, vs, strlen(vs), fs, strlen(fs));
		free(vs);
		free(fs);
		return shad;
	}
	catch(Error)
	{
		// Failure
		free(vs);
		free(fs);
		throw;
	}
}
	
uint32_t Shader::compile(int type, const char *src)
{
	// Create a new shader
	uint32_t sh = glCreateShader(type);

	// Provide the source
	glShaderSource(sh, 1, &src, NULL);

	// Compile it
	glCompileShader(sh);

	// Check the result
	int status;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
	if(!status)
	{
		// Failure, get the reason
		int len;
		glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &len);

		char *buf = (char*)malloc(len);
		glGetShaderInfoLog(sh, len, &len, buf);

		// Copy into a string object
		Str s(buf);
		free(buf);

		// Destroy the shader and throw an error
		glDeleteShader(sh);

		E::ShaderCompile("%s", s.ptr());
	}

	// Compilation was successful
	return sh;
}

void Shader::compile()
{
	// If already compiled, do nothing
	if(_prg)
		return;

	ASSERT(Display::get_window(), "Shader::compile(): No mode currently set")

	Log::log("Compiling shader: %s", _name.ptr());

	// Compile the vertex shader
	uint32_t vs = compile(GL_VERTEX_SHADER, _vs_src);

	// Compile the fragment shader
	uint32_t fs = 0;
	try
	{
		fs = compile(GL_FRAGMENT_SHADER, _fs_src);
	}
	catch(Error)
	{
		// Discard the vertex shader
		glDeleteShader(vs);
		vs = 0;
		throw;
	}

	// Both shaders compiled properly
	try
	{
		// Create a program
		_prg = glCreateProgram();

		// Attach the shaders
		glAttachShader(_prg, vs);
		glAttachShader(_prg, fs);

		// Link it
		glLinkProgram(_prg);

		// Check status
		int status;
		glGetProgramiv(_prg, GL_LINK_STATUS, &status);
		if(!status)
		{
			// Failure, get the reason
			int len;
			glGetProgramiv(_prg, GL_INFO_LOG_LENGTH, &len);

			char *buf = (char*)malloc(len);
			glGetProgramInfoLog(_prg, len, &len, buf);

			// Copy into a string object
			Str s(buf);
			free(buf);

			// Throw an error
			E::ShaderLink("%s", s.ptr());
		}
	}
	catch(Error)
	{
		// Discard everything
		glDeleteShader(vs);
		glDeleteShader(fs);

		glDeleteProgram(_prg);
		_prg = 0;

		throw;
	}

	// Success
	// Get the number of attributes and uniforms
	int num_attr, num_uni;

	glGetProgramiv(_prg, GL_ACTIVE_ATTRIBUTES, &num_attr);
	glGetProgramiv(_prg, GL_ACTIVE_UNIFORMS, &num_uni);

	#ifdef DBG
	Log::debug("Success. %i attributes, %i uniforms", num_attr, num_uni);
	#endif

	// Fill in the uniforms info
	for(int c = 0; c<num_uni; c++)
	{
		char buf[1024];
		int sz;
		GLenum type;
		glGetActiveUniform(_prg, c, 1024, NULL, &sz, &type, buf);

		// Get its index
		int i = glGetUniformLocation(_prg, buf);

		#ifdef DBG
		Log::debug("Uniform %i: '%s' (Size: %i) (Type: %i)", i, buf, sz, type);
		#endif

		// Add it
		ShaderUniformAttrib *uni = new ShaderUniformAttrib();
		uni->index = i;
		uni->name = buf;
		uni->type = type;
		uni->size = sz;

		_uniforms.add(uni);
	}

	// Fill in the attributes info
	for(int c = 0; c<num_attr; c++)
	{
		char buf[1024];
		int sz;
		GLenum type;
		glGetActiveAttrib(_prg, c, 1024, NULL, &sz, &type, buf);

		// Get its index
		int i = glGetAttribLocation(_prg, buf);

		#ifdef DBG
		Log::debug("Attribute %i: '%s' (Size: %i) (Type: %i)", i, buf, sz, type);
		#endif
		
		// Add it
		ShaderUniformAttrib *attr = new ShaderUniformAttrib();
		attr->index = i;
		attr->name = buf;
		attr->type = type;
		attr->size = sz;

		_attribs.add(attr);
	}

	// Discard our reference to the shaders
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::bind()
{
	ASSERT(Display::get_window(), "Shader::bind(): No mode currently set")

	// Compile the shader, if needed
	compile();

	// Bind it
	glUseProgram(_prg);
}

void Shader::unbind()
{
	glUseProgram(0);
}

ShaderUniformAttrib *Shader::get_uniform(const Str& name, int type)
{
	// Look for the uniform
	for(int c = 0; c<_uniforms.size(); c++)
	{
		ShaderUniformAttrib *uni = _uniforms[c];

		if(uni->name==name)
		{
			// Found it
			// Validate the type
			if(uni->type!=type)
				E::BadUniformType("'%s' is not of the expected type", name.ptr());

			return uni;
		}
	}

	// Not found
	E::NoUniform("%s", name.ptr());
	return NULL;
}
	
ShaderUniformAttrib *Shader::get_attrib(const Str& name)
{
	// Look for the attribute
	for(int c = 0; c<_attribs.size(); c++)
	{
		ShaderUniformAttrib *attrib = _attribs[c];

		if(attrib->name==name)
			// Found it
			return attrib;
	}

	// Not found
	return NULL;
}
	
int Shader::get_attribute_index(const Str& name)
{
	ShaderUniformAttrib *attr = get_attrib(name);
	if(!attr)
		return -1;

	return attr->index;
}

void Shader::set_uniform(const Str& name, const Matrix& mat)
{
	// Set a matrix uniform
	glUniformMatrix4fv(get_uniform(name, GL_FLOAT_MAT4)->index, 1, 0, mat.transpose().val);
}

void Shader::set_uniform(const Str& name, int index)
{
	glUniform1i(get_uniform(name, GL_SAMPLER_2D)->index, index);
}


}
