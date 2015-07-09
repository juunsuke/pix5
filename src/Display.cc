#include "pix5.h"
#include <SDL.h>
#include <GL/glew.h>


namespace PIX {

namespace Input { void reset(); }

namespace Display {


static SDL_Window *_window = NULL;
static SDL_GLContext _gl = NULL;
// Window and context for the current mode

static VideoMode _cur_mode;
// Current video mode

static Camera _2d_cam;
// A 2D camera for the current mode


void init()
{
}

void done()
{
	// Unset any mode
	unset_mode();
}

static void set_viewport(int w, int h)
{
	// Set the OpenGL viewport
	glViewport(0, 0, w, h);

	// Recalculate the 2D camera's projection matrix
	_2d_cam.mat = Matrix::ortho2d(w, h);

}

void handle_resize(int width, int height)
{
	// The window was resized
	Log::log("The window has been resized to %ix%i", width, height);
	_cur_mode.width = width;
	_cur_mode.height = height;

	set_viewport(width, height);

	// Reset the input system
	Input::reset();
}

void handle_move(int x, int y)
{
	// The window was moved
	Log::log("The window was moved to: %i,%i", x, y);
	_cur_mode.x = x;
	_cur_mode.y = y;
}

void unset_mode()
{
	// Unset any currently set mode
	if(_gl)
	{
		SDL_GL_DeleteContext(_gl);
		_gl = NULL;
	}

	if(_window)
	{
		Log::log("Closing window");

		SDL_DestroyWindow(_window);
		_window = NULL;
	}

	_cur_mode.clear();
}

void set_mode(const VideoMode& vm)
{
	// Unset the current mode if any
	unset_mode();

	// Bail out if the mode has a type of None
	if(vm.type==VideoModeType::None)
		return;

	// Calc the flags
	uint32_t flags = SDL_WINDOW_OPENGL;
	int width = vm.width;
	int height = vm.height;
	const char *mode_desc = "";

	switch(vm.type)
	{
		case VideoModeType::Window:
			// Normal window
			mode_desc = "Windowed";
			break;

		case VideoModeType::Resizable:
			// Resizable window
			mode_desc = "Resizable";
			flags |= SDL_WINDOW_RESIZABLE;
			break;

		case VideoModeType::Desktop:
			// Windowed fullscreen
			mode_desc = "Windowed fullscreen";
			flags |= SDL_WINDOW_BORDERLESS;
			width = 0;
			height = 0;
			break;

		case VideoModeType::Fullscreen:
			// Fullscreen
			mode_desc = "Fullscreen";
			flags |= SDL_WINDOW_FULLSCREEN;
			break;

		default:
			E::Assert("set_mode(): Invalid video mode type");
			break;
	}

	Log::log("Setting new video mode: %s %ix%i", mode_desc, width, height);

	// Try and set the mode

	try
	{
		// Create the window
		_window = SDL_CreateWindow(get_app_name(), vm.x, vm.y, width, height, flags);
		if(!_window)
			E::SetMode("Could not create window: %s", SDL_GetError());

		// Create the OpenGL context
		_gl = SDL_GL_CreateContext(_window);
		if(!_gl)
			E::SetMode("Error creating OpenGL context: %s", SDL_GetError());

		// Initialize GLEW
		GLenum err = glewInit();
		if(err!=GLEW_OK)
			E::SetMode("Error initializing GLEW: %s", glewGetErrorString(err));

		// Set VSync
		SDL_GL_SetSwapInterval(vm.vsync ? 1 : 0);

		// Setup the current mode
		_cur_mode = vm;
		SDL_GetWindowSize(_window, &_cur_mode.width, &_cur_mode.height);
		SDL_GetWindowPosition(_window, &_cur_mode.x, &_cur_mode.y);

		// Reset the OpenGL state
		set_viewport(_cur_mode.width, _cur_mode.height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Reset the input system
		Input::reset();
	}
	catch(Error)
	{
		unset_mode();
		throw;
	}

	Log::log("Video mode successfully set.  Actual resolution: %ix%i", _cur_mode.width, _cur_mode.height);

	int sz = get_max_texture_size();
	Log::log("Maximum texture size supported by the GPU: %ix%i", sz, sz);

	if(has_npot())
		Log::log("The GPU supports non-power-of-two (NPOT) textures");
	else
		Log::log("The GPU *DOES NOT* support non-power-of-two (NPOT) textures");

	Log::log("Number of texture units supported by the GPU: %i", get_texture_units());		
}

SDL_Window *get_window()
{
	return _window;
}

VideoMode get_mode()
{
	return _cur_mode;
}

bool has_focus()
{
	ASSERT(_window, "Display::has_focus(): No mode currently set")

	return (SDL_GetWindowFlags(_window)&SDL_WINDOW_INPUT_FOCUS) ? true : false;
}

bool is_shown()
{
	ASSERT(_window, "Display::is_shown(): No mode currently set")

	return (SDL_GetWindowFlags(_window)&SDL_WINDOW_SHOWN) ? true : false;
}

bool is_minimized()
{
	ASSERT(_window, "Display::is_minimized(): No mode currently set")

	return (SDL_GetWindowFlags(_window)&SDL_WINDOW_MINIMIZED) ? true : false;
}

bool is_maximized()
{
	ASSERT(_window, "Display::is_maximized(): No mode currently set")

	return (SDL_GetWindowFlags(_window)&SDL_WINDOW_MAXIMIZED) ? true : false;
}

void clear(const Color& col)
{
	ASSERT(_window, "Display::clear(): No mode currently set")

	// Clear the back buffer
	glClearColor(col.fr(), col.fg(), col.fb(), col.fa());
	glClear(GL_COLOR_BUFFER_BIT);
}

void swap()
{
	ASSERT(_window, "Display::swap(): No mode currently set")

	SDL_GL_SwapWindow(_window);
}

void show_cursor(bool show)
{
	ASSERT(_window, "Display::show_cursor(): No mode currently set")

	SDL_ShowCursor(show ? 1 : 0);
}

bool cursor_visible()
{
	ASSERT(_window, "Display::cursor_visible(): No mode currently set")

	return SDL_ShowCursor(-1)==1 ? true : false;
}

int get_max_texture_size()
{
	ASSERT(_window, "Display::get_max_texture_size(): No mode currently set")

	GLint val;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);

	return val;
}

bool has_npot()
{
	ASSERT(_window, "Display::has_npot(): No mode currently set")
	
	return GLEW_ARB_texture_non_power_of_two ? true : false;
}

int get_texture_units()
{
	ASSERT(_window, "Display::has_npot(): No mode currently set")

	GLint val;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &val);

	return val;
}

const Camera& get_2d_camera()
{
	return _2d_cam;
}

const Matrix& get_2d_matrix()
{
	return _2d_cam.mat;
}

void test_draw(int x, int y, int w, int h, Texture *tex, const Color& col)
{
	ASSERT(_window, "Display::test_draw(): No mode currently set")

	// Bind/unbind the texture
	if(tex)
		tex->bind(0);
	else
		Texture::unbind(0);

	// Setup the matrices
	_2d_cam.set();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glColor4f(col.fr(), col.fg(), col.fb(), col.fa());

		if(tex)
			glTexCoord2f(0, 0);
		glVertex3f(x, y, 0);

		if(tex)
			glTexCoord2f(1, 0);
		glVertex3f(x+w, y, 0);

		if(tex)
			glTexCoord2f(1, 1);
		glVertex3f(x+w, y+h, 0);

		if(tex)
			glTexCoord2f(0, 1);
		glVertex3f(x, y+h, 0);
	glEnd();
}

void test_draw(int x, int y, Texture *tex, const Color& col)
{
	ASSERT(tex, "Display::test_draw(): NULL texture provided with the no-size version of the function")

	test_draw(x, y, tex->width(), tex->height(), tex, col);
}

void draw(int first, int count)
{
	ASSERT(_window, "Display::draw(): No mode currently set")

	glDrawArrays(GL_QUADS, first, count);
}



}}

