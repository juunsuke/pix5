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

Renderer *renderer();
// Retrieve the renderer instance

void render();
// Shortcut for renderer()->render()


}

