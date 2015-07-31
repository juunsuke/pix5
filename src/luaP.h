#pragma once



void *luaP_createudata(lua_State *lua, int size, const char *name);
// Create a new userdata of the given size
// If 'name' is non-NULL, associate a registry metatable with the userdata
// Returns the allocated pointer
// The new userdata is pushed onto the stack

void luaP_createudata(lua_State *lua, const char *name, void *p);
// Create a new userdata to hold a PIX instance
// 'name' is the registry metatable name
// 'p' is the pointer to associate with the userdata
// The new userdata is pushed onto the stack

Texture *luaP_checktexture(lua_State *lua, int narg);
// Check for PIX object wrappers

Rect luaP_checkrect(lua_State *lua, int narg);
// Get a rectangle parameter




