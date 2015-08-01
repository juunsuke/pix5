#pragma once


class JoyButton
{
public:

	int index;
	// Button index

	bool val;
	// Button value (true = pushed down)


	JoyButton(int index)
	{
		this->index = index;
		val = false;
	}

	JoyButton(const JoyButton& o)
	{
		index = o.index;
		val = o.val;
	}

	~JoyButton()
	{
	}
};


