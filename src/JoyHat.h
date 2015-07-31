#pragma once


namespace HatDirection
{
	enum Type
	{
		// Hat direction numbers follow the keypad's number directions, with 0 for centered
		Center,			// 0
		DownLeft,		// 1
		Down,			// 2
		DownRight,		// 3
		Left,			// 4
		Unused,
		Right,			// 6
		UpLeft,			// 7
		Up,				// 8
		UpRight			// 9
	};
}



class JoyHat: public Obj
{
	PIXOBJ("JoyHat")
public:

	int index;
	// Hat index

	HatDirection::Type val;
	// Hat direction


	JoyHat(int index)
	{
		this->index = index;
		val = HatDirection::Center;
	}

	JoyHat(const JoyHat& o)
	{
		index = o.index;
		val = o.val;
	}

	~JoyHat()
	{
	}


	inline bool left() const
	{
		return val==HatDirection::DownLeft || val==HatDirection::Left || val==HatDirection::UpLeft;
	}

	inline bool right() const
	{
		return val==HatDirection::DownRight || val==HatDirection::Right || val==HatDirection::UpRight;
	}

	inline bool up() const
	{
		return val==HatDirection::UpLeft || val==HatDirection::Up || val==HatDirection::UpRight;
	}

	inline bool down() const
	{
		return val==HatDirection::DownLeft || val==HatDirection::Down || val==HatDirection::DownRight;
	}


};


