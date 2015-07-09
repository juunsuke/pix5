#pragma once

namespace Display
{


void unset_mode();
// Unset any currently set mode

void set_mode(const VideoMode& vm);
// Set/change the current video mode
// The first thing this function does is unset the previously set mode
// If an error occurs while setting the new mode, the previous one will
// not be restored

SDL_Window *get_window();
// Get the SDL Window pointer for the current mode
// Handle with care
// Valid until a mode change is performed
// Returns NULL if no mode is currently set

VideoMode get_mode();
// Get a copy of the current mode information class

bool has_focus();
bool is_shown();
bool is_minimized();
bool is_maximized();
// Returns information about the window
// Calling these with no mode set will result in undefined behaviour

void clear(const Color& col = 0);
// Clear the back buffer using the given color

void swap();
// Swap the buffers

void show_cursor(bool show);
// Show/hide the mouse cursor

bool cursor_visible();
// Returns true if the cursor if visible, false if hidden

int get_max_texture_size();
// Return the maximum texture size supported by the GPU
// A mode must currently be set
// The value applies to both width and height

bool has_npot();
// Returns wether the GPU supports non-power-of-two (NPOT) textures
// A mode must currently be set

int get_texture_units();
// Returns the number of texture units supported by the GPU
// A mode must currently be set

const Camera& get_2d_camera();
// Retrieve a 2D camera for the current mode
// This returns a reference to an internal camera for pixel-perfect 1:1
// 2D coordinates
// The camera/matrix are not calculated when this function is called, but rather
// whenever a mode change or window resize occurs

const Matrix& get_2d_matrix();
// Retrieves the 2D camera's matrix

void test_draw(int x, int y, int w, int h, Texture *tex = NULL, const Color& col = Color(1.0f, 1.0f, 1.0f));
void test_draw(int x, int y, Texture *tex, const Color& col = Color(1.0f, 1.0f, 1.0f));
// Draw a single test rectangle
// This function uses the OpenGL direct draw functions, as such performances
// are poor and not supported everywhere
// As such, as its name implies, it should only be used as a test function
// If a texture is provided, it will be bound, and not unbound
// If no texture is provided, any curreny texture will be unbound, and not restored
// The internal 2D camera will be used to set the projection matrix
// The modelview matrix will be reset to identity
// Both matrices will not be restored to their former state
// The second version of the function takes the dimensions from the texture, which must not be NULL

void draw(int first, int count);


}

