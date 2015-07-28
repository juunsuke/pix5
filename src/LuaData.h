#pragma once


namespace LuaType
{
	enum Type
	{
		Nil,
		Number,
		Bool,
		String,
		Table,
		Function,
		UserData,
		LightUserData,
		Thread
	};
}


class LuaData
{
	Lua *_lua;
	// The lua state

	LuaData *_master;
	// Master LuaData, for nested tables

	LuaType::Type _type;
	// Data type

	double _num;
	bool _bool;
	Str _str;
	// For basic types

	void *_user_data;
	// For normal/light user datas

	int _func;
	// For functions, is a reference in the registry

	int _int_key;
	Str _str_key;
	bool _is_str_key;
	// Integer/string key, when is an entry of a higher level table

	List<LuaData*> _subs;
	// Sub-data elements, for tables


	void extract_table(int i);
	// Extract a table

public:

	LuaData(Lua *lua, int i, LuaData *master);
	~LuaData();

	inline LuaType::Type type() { return _type; }
	// Get the data type

	Str str_type();
	// Get the data type, as a string

	inline bool is_nil()				{ return _type==LuaType::Nil; }
	inline bool is_number()				{ return _type==LuaType::Number; }
	inline bool is_bool()				{ return _type==LuaType::Bool; }
	inline bool is_string()				{ return _type==LuaType::String; }
	inline bool is_table()				{ return _type==LuaType::Table; }
	inline bool is_function()			{ return _type==LuaType::Function; }
	inline bool is_user_data()			{ return _type==LuaType::UserData; }
	inline bool is_light_user_data()	{ return _type==LuaType::LightUserData; }
	inline bool is_thread()				{ return _type==LuaType::Thread; }


	inline double get_number()			{ return is_number() ? _num : 0; }
	inline int get_int()				{ return (int)get_number(); }
	inline bool get_bool()				{ return is_bool() ? _bool : false; }
	inline Str get_str()				{ return _str; }
	inline void *get_user_data()		{ return (is_user_data() || is_light_user_data()) ? _user_data : NULL; }
	// Get the simple data values

	void get_func();
	// Push the function on top of the stack
	// Will throw an error if not a function

	#ifdef DBG
	void log(int spacing = 0);
	// Print out on the debug log the table data type and value
	// Will recursively print tables
	#endif
};



