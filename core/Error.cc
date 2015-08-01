#include "pix5core.h"


Error::Error(uint32_t code, const Str& name, const Str &desc, const Str& message)
{
	this->code = code;
	this->name = name;
	this->desc = desc;
	this->message = message;

	#ifdef DBG
	Log::error(*this);
	#endif
}



