#pragma once


class JoyAxis: public Obj
{
	PIXOBJ("JoyAxis")
public:

	int index;
	// Axis index

	float val;
	// Axis value, between -1 and 1

	float dead_zone;
	// Axis dead zone


	JoyAxis(int index)
	{
		this->index = index;
		val = 0;
		dead_zone = 0.1f;
	}

	JoyAxis(const JoyAxis& o)
	{
		index = o.index;
		val = o.val;
		dead_zone = o.dead_zone;
	}

	~JoyAxis()
	{
	}
};



