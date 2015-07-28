#include "pix5.h"

namespace PIX {


LuaData::LuaData(Lua *lua, int i, LuaData *master)
{
	_lua = lua;
	_master = master;

	if(i<0)
		i = lua->get_top()+i+1;

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
			_type = LuaType::Table;
			extract_table(i);
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

	_subs.clear_del();
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

void LuaData::extract_table(int i)
{
	printf("Extracting table @%i\n", i);

	// Check all the table's fields
	_lua->push_nil();
	while(_lua->next(i))
	{
		// For now, only number and string keys are supported
		if(_lua->type(-2)==LUA_TNUMBER)
		{
			// Make sure it's an integer
			double num = _lua->to_number(-2);
			int inum = (int)num;

			if(num == (double)inum)
			{
				// Create a new data
				LuaData *sub = new LuaData(_lua, -1, _master ? _master : this);
				sub->_is_str_key = false;
				sub->_int_key = inum;

				_subs.add(sub);
			}
			else
			{
				#ifdef DBG
				Log::debug("LuaData::extract_table(): A table entry was ignored because its key is not an integer or a string");
				#endif
			}
		}
		else if(_lua->type(-2)==LUA_TSTRING)
		{
			// Create a new data
			LuaData *sub = new LuaData(_lua, -1, _master ? _master : this);
			sub->_is_str_key = true;
			sub->_str_key = _lua->to_string(-2);

			_subs.add(sub);
		}
		else
		{
			// Ignore it
			#ifdef DBG
			Log::debug("LuaData::extract_table(): A table entry was ignored because its key is not an integer or a string");
			#endif
		}

		_lua->pop();
	}
}

#ifdef DBG
void LuaData::log(int spacing)
{
	Str s;
	for(int c = 0; c<spacing; c++)
		s += " ";

	s += Str::build("<%s> ", str_type().ptr());

	switch(_type)
	{
		case LuaType::Number:
			s += Str::build("%f", get_number());
			break;

		case LuaType::String:
			s += Str::build("\"%s\"", get_str().ptr());
			break;

		case LuaType::Bool:
			s += get_bool() ? "true" : "false";
			break;

		default:
			break;
	}

	Log::debug(s);

	if(_type==LuaType::Table)
	{
		// Log the subs
		s = "";
		for(int c = 0; c<spacing; c++)
			s += " ";

		s += "{";

		Log::debug(s);

		for(int c = 0; c<_subs.size(); c++)
		{
			LuaData *sub = _subs[c];

			s = "  ";
			for(int c = 0; c<spacing; c++)
				s += " ";

			if(sub->_is_str_key)
				s += Str::build("[\"%s\"] :", sub->_str_key.ptr());
			else
				s += Str::build("[%i] :", sub->_int_key);

			Log::debug(s);
			sub->log(spacing+4);
		}
		
		s = "";
		for(int c = 0; c<spacing; c++)
			s += " ";

		s += "}";

		Log::debug(s);
	}
}
#endif


}
