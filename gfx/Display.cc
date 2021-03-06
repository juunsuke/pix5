#include "pix5gfx.h"
#include "bi_data.h"
#include <SDL.h>
#include <GL/glew.h>


namespace Input { void reset(); }

namespace Display {


static SDL_Window *_window = NULL;
static SDL_GLContext _gl = NULL;
// Window and context for the current mode

static VideoMode _cur_mode;
// Current video mode

static int _frames = 0;
static uint32_t _last_time = 0;
// Used by the FPS counter

static Renderer *_renderer = NULL;
// The renderer

static Font *_def_font = NULL;
// The default font

static SDL_Cursor *_cur_cur = NULL;
// Current cursor


void init()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	_window = NULL;
	_gl = NULL;
	_cur_mode.type = VideoModeType::None;
	_renderer = NULL;

	// Load the default font
	_def_font = Font::load(data_LiberationSans_Regular_ttf, data_LiberationSans_Regular_ttf_len, 12);
}

void done()
{
	// Unset any mode
	unset_mode();

	if(_def_font)
	{
		delete _def_font;
		_def_font = NULL;
	}
}

static void set_viewport(int w, int h)
{
	// Set the OpenGL viewport
	glViewport(0, 0, w, h);

	// Rebuild the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -1, 1);
		
	glMatrixMode(GL_MODELVIEW);
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
	#ifdef DBG
	Log::debug("The window was moved to: %i,%i", x, y);
	#endif
	_cur_mode.x = x;
	_cur_mode.y = y;
}

static void free_cur()
{
	if(_cur_cur)
	{
		SDL_FreeCursor(_cur_cur);
		_cur_cur = NULL;
	}
}

void unset_mode()
{
	// Release all the video data
	VideoData::release_all();

	free_cur();

	// Unset any currently set mode
	if(_renderer)
	{
		_renderer->done();
		delete _renderer;
		_renderer = NULL;
	}

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

void set_renderer(RendererType::Type type)
{
	// Is there already a renderer ?
	if(_renderer)
	{
		// Do nothing if it's the same type
		if(_cur_mode.renderer==type)
			return;

		// Shut it down
		_renderer->done();
		delete _renderer;
		_renderer = NULL;
	}

	_cur_mode.renderer = type;

	if(_cur_mode.renderer==RendererType::Shader)
	{
		// Create an shader renderer
		try
		{
			Log::log("Using the OpenGL Shader renderer");
			_renderer = new ShaderRenderer();
			_renderer->init();

			return;
		}
		catch(Error)
		{
			Log::log("Failure to create the Shader renderer, falling back to a lower renderer");
			_cur_mode.renderer = RendererType::VBO;
		}
	}

	if(_cur_mode.renderer==RendererType::VBO)
	{
		// Create an VBO renderer
		try
		{
			Log::log("Using the OpenGL VBO renderer");
			_renderer = new VBORenderer();
			_renderer->init();

			return;
		}
		catch(Error)
		{
			Log::log("Failure to create the VBO renderer, falling back to a lower renderer");
			_cur_mode.renderer = RendererType::Immediate;
		}
	}

	if(_cur_mode.renderer==RendererType::Immediate)
	{
		// Create an immediate mode renderer
		Log::log("Using the OpenGL Immediate Mode renderer");
		_renderer = new ImmRenderer();
		_renderer->init();

		return;
	}

	ASSERT(0, "Display::set_renderer(): Invalid renderer type")
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

			SDL_DisplayMode dm;
			SDL_GetDesktopDisplayMode(0, &dm);
			width = dm.w;
			height = dm.h;
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

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Reset the input system
		Input::reset();

		// Create the renderer
		set_renderer(_cur_mode.renderer);
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

	_frames = 0;
	_last_time = 0;
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

	// Do FPS calculations
	uint32_t now = SDL_GetTicks();

	if(_last_time==0)
	{
		// Init
		_last_time = now;
	}
	else
	{
		_frames++;

		// Display the FPS every second on the title bar
		if((now-_last_time)>=1000)
		{
			#ifdef DBG
			float fps = (float)((double)_frames/((double)(now-_last_time)/1000.0f));
			SDL_SetWindowTitle(_window, Str::build("%s - %f frames/second", get_app_name().ptr(), fps));
			#endif
	
			_last_time = now;
			_frames = 0;
		}
	}
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

Renderer *renderer()
{
	ASSERT(_window, "Display::renderer(): No mode currently set")

	return _renderer;
}

void render()
{
	ASSERT(_window, "Display::render(): No mode currently set")

	_renderer->render();
}

Font *default_font()
{
	return _def_font;
}



void move_mouse(int x, int y)
{
	ASSERT(_window, "move_mouse(): No mode currently set")

	SDL_WarpMouseInWindow(_window, x, y);
}

void show_cursor(bool show)
{
	SDL_ShowCursor(show ? 1 : 0);
}

void set_cursor(SystemCursor::Type cur)
{
	// Free any current cursor
	free_cur();

	// Create a system cursor
	_cur_cur = SDL_CreateSystemCursor((SDL_SystemCursor)cur);
	if(!_cur_cur)
		E::SDL("Error creating system cursor: %s", SDL_GetError());

	// Set it
	SDL_SetCursor(_cur_cur);
}

static void create_mono_cursor(Texture *tex, int hx, int hy)
{
	// Create buffers for the data and mask
	int cw = (tex->width()+7)/8;
	int ch = tex->height();

	Buffer<uint8_t> data(cw*ch, true);
	Buffer<uint8_t> mask(cw*ch, true);

	for(int y = 0; y<tex->height(); y++)
	{
		int pos = y*cw;

		for(int x = 0; x<tex->width(); x++)
		{
			switch(tex->get_pixel_fast(x, y))
			{
				case 0xFF000000:
					// Black
					data[pos+x/8] |= 1<<(7-x%8);
					mask[pos+x/8] |= 1<<(7-x%8);
					break;

				case 0xFFFFFFFF:
					// White
					mask[pos+x/8] |= 1<<(7-x%8);
					break;

				default:
					// Transparant
					break;
			}
		}
	}

	_cur_cur = SDL_CreateCursor(data, mask, cw*8, ch, hx, hy);

	if(!_cur_cur)
		E::SDL("Error creating mono cursor: %s", SDL_GetError());
}

static void create_color_cursor(Texture *tex, int hx, int hy)
{
	// Create a surface out of the texture
	SDL_Surface *sur = SDL_CreateRGBSurface(0, tex->width(), tex->height(), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	if(!sur)
		E::SDL("Input::set_cursor(): Error creating a surface: %s", SDL_GetError());

	// Copy the texture data to it
	SDL_LockSurface(sur);
	
	uint8_t *ptr = (uint8_t*)sur->pixels;

	for(int y = 0; y<tex->height(); y++)
	{
		for(int x = 0; x<tex->width(); x++)
			((uint32_t*)ptr)[x] = tex->get_pixel_fast(x, y);

		ptr += sur->pitch;
	}

	SDL_UnlockSurface(sur);

	// Create a color cursor
	_cur_cur = SDL_CreateColorCursor(sur, hx, hy);
	SDL_FreeSurface(sur);

	if(!_cur_cur)
		E::SDL("Error creating color cursor: %s", SDL_GetError());

}

void set_cursor(Texture *tex, int hx, int hy, bool mono)
{
	// Free any current cursor
	free_cur();

	// Create the cursor
	if(mono)
		create_mono_cursor(tex, hx, hy);
	else
		create_color_cursor(tex, hx, hy);
		
	// Set it
	SDL_SetCursor(_cur_cur);
}


}

