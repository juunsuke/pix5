#pragma once

class Joystick: public Obj
{
	PIXOBJ("Joystick")

	void copy(const Joystick& o)
	{
		id = o.id;
		instance_id = o.instance_id;
		name = o.name;
		mapped_name = name;

		for(int c = 0; c<o.axis.size(); c++)
			axis.add(new JoyAxis(*o.axis.get(c)));

		for(int c = 0; c<o.buts.size(); c++)
			buts.add(new JoyButton(*o.buts.get(c)));

		for(int c = 0; c<o.hats.size(); c++)
			hats.add(new JoyHat(*o.hats.get(c)));
	}

	void clear()
	{
		axis.clear_del();
		buts.clear_del();
		hats.clear_del();
	}

public:

	int id;
	// PIX ID for the joystick, guaranteed to be unique
	// If the same joystick is unplugged/replugged several times, a new ID will
	// be generated each time

	int instance_id;
	// Joystick instance ID, given by SDL

	_SDL_Joystick *_sj;
	// SDL joystick instance, always NULL in user-obtained instances of this class

	Str name;
	// Joystick name

	Str mapped_name;
	// Input actions name mapping for the joystick

	List<JoyAxis*> axis;
	// List of the joystick's axis

	List<JoyButton*> buts;
	// List of the joystick's buttons

	List<JoyHat*> hats;
	// List of the joystick's hats


	Joystick()
	{
		id = -1;
		instance_id = -1;
		_sj = NULL;
	}

	Joystick(const Joystick& o)
	{
		// Copy constructor
		copy(o);
		_sj = NULL;
	}

	~Joystick()
	{
		clear();
	}

	Joystick& operator=(const Joystick& o)
	{
		// Affectation
		clear();
		copy(o);

		return *this;
	}
};

