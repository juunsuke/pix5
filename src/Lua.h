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

	inline lua_State *state()							{ return _lua; }

	inline void push_value(int i)						{ lua_pushvalue(_lua, i); }
	inline void push_nil()								{ lua_pushnil(_lua); }

	inline double to_number(int i = -1)					{ return lua_tonumber(_lua, i); }
	inline bool to_bool(int i = -1)						{ return lua_toboolean(_lua, i) ? true : false; }
	inline Str to_string(int i = -1)					{ return lua_tostring(_lua, i); }
	inline void *to_user_data(int i = -1)				{ return lua_touserdata(_lua, i); }

	inline int get_top()								{ return lua_gettop(_lua); }
	inline void get_global(const char *name)			{ lua_getglobal(_lua, name); }
	inline void pop(int num = 1)						{ lua_pop(_lua, num); }
	inline void remove(int i)							{ lua_remove(_lua, i); }
	inline int type(int i = -1)							{ return lua_type(_lua, i); }
	inline void rawgeti(int t, int r)					{ lua_rawgeti(_lua, t, r); }
	inline void rawseti(int t, int r)					{ lua_rawseti(_lua, t, r); }
	inline int next(int t)								{ return lua_next(_lua, t); }

	inline int ref(int t)								{ return luaL_ref(_lua, t); }
	inline void unref(int t, int r)						{ luaL_unref(_lua, t, r); }
	inline int ref_registry()							{ return ref(LUA_REGISTRYINDEX); }
	inline void unref_registry(int r)					{ unref(LUA_REGISTRYINDEX, r); }
	inline void get_registry(int r)						{ rawgeti(LUA_REGISTRYINDEX, r); }
	inline void set_registry(int r)						{ rawseti(LUA_REGISTRYINDEX, r); }

	bool pcall(int narg, int nres);
	inline void error()									{ lua_error(_lua); }

	inline Str get_error()
	{
		// Get and pop an error string
		Str e = to_string();
		pop();
		return e;
	}


	class LuaData *get_data(const Str& name);
	// Get a global variable from the lua state
	// 'name' is the name of the variable, which must be global
	// If the variable doesn't exist, a valid LuaData will be returned, holding nil
	// If the retrieved variable is a table, it will be completely parsed and retrieved
	// Circular references in tables are handled properly


	static Lua *get(lua_State *ls)					{ return (Lua*)lua_touserdata(ls, lua_upvalueindex(1)); }
};


