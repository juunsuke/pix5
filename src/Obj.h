#pragma once

#ifdef DBG
	#define PIXOBJ(name) const char *__obj_get_name() { return "PIX::" name; }
	#define OBJ(name) const char *__obj_get_name() { return name; }
#else
	#define PIXOBJ(name)
	#define OBJ(name)
#endif


class Obj
{
public:

	#ifdef DBG

		Obj *__obj_prev;
		Obj *__obj_next;

		Obj();
		virtual ~Obj();

		virtual const char *__obj_get_name() = 0;

	#else

		Obj() { }
		virtual ~Obj() { }

	#endif
};

