#pragma once


class Lua
{
	lua_State *_lua;
	// The state

public:

	Lua();
	~Lua();

	void load(const Str& fname, bool run = true);
	// Load a LUA source file and leave a function pointer on the stack
	// If 'run' is true, automatically call it


	inline void push_value(int i)					{ lua_pushvalue(_lua, i); }

	inline Str to_string(int i = -1)				{ return lua_tostring(_lua, i); }

	inline int get_top()							{ return lua_gettop(_lua); }
	inline void get_global(const char *name)		{ lua_getglobal(_lua, name); }
	inline void pop(int num = 1)					{ lua_pop(_lua, num); }
	inline void remove(int i)						{ lua_remove(_lua, i); }

	inline bool pcall(int narg, int nres)			{ return lua_pcall(_lua, narg, nres, 0)==0; }
	inline void error()								{ lua_error(_lua); }

	inline Str get_error()
	{
		// Get and pop an error string
		Str e = to_string();
		pop();
		return e;
	}


	static Lua *get(lua_State *ls)					{ return (Lua*)lua_touserdata(ls, lua_upvalueindex(1)); }
};


