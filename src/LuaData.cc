#include "pix5.h"

namespace PIX {


LuaData::LuaData(Lua *lua, int i)
{
	_lua = lua;

	// Check the type and extract the value
	switch(_lua->type(i))
	{
		case LUA_TNUMBER:
			// Double number
			_type = LuaType::Number;
			_num = _lua->to_number(i);
			break;

		case LUA_TBOOLEAN:
			// Boolean
			_type = LuaType::Bool;
			_bool = _lua->to_bool(i);
			break;

		case LUA_TSTRING:
			// String
			_type = LuaType::String;
			_str = _lua->to_string(i);
			break;

		case LUA_TUSERDATA:
			// Full user data
			_type = LuaType::UserData;
			_user_data = _lua->to_user_data(i);
			break;

		case LUA_TLIGHTUSERDATA:
			// Light user data
			_type = LuaType::LightUserData;
			_user_data = _lua->to_user_data(i);
			break;

		case LUA_TFUNCTION:
			// Function, make a copy in the registry
			_type = LuaType::Function;
			_lua->push_value(i);
			_func = _lua->ref_registry();
			break;

		case LUA_TTHREAD:
			// Thread, mark the type but extract no data
			_type = LuaType::Thread;
			break;

		case LUA_TTABLE:
			break;

		case LUA_TNIL:
		default:
			// Nil or unrecognized, just mark as nil
			_type = LuaType::Nil;
			break;
	}
}

LuaData::~LuaData()
{
	if(_type==LuaType::Function)
	{
		// Unreference the function from the registre
		_lua->unref_registry(_func);
	}
}

Str LuaData::str_type()
{
	switch(_type)
	{
		case LuaType::Nil:				return "Nil";
		case LuaType::Number:			return "Number";
		case LuaType::Bool:				return "Bool";
		case LuaType::String:			return "String";
		case LuaType::Table:			return "Table";
		case LuaType::Function:			return "Function";
		case LuaType::UserData:			return "UserData";
		case LuaType::LightUserData:	return "LightUserData";
		case LuaType::Thread:			return "Thread";
	}

	return "Unknown";
}

void LuaData::get_func()
{
	// Make sure its a function
	if(_type!=LuaType::Function)
		E::Lua("LuaData::get_func(): The data type is not a function");

	// Get the function
	_lua->get_registry(_func);
}


}
