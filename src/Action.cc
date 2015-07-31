#include "pix5.h"

namespace PIX {

namespace Input
{
	bool is_joy_button_down(const Str& joy_name, int but);
	bool is_joy_hat(const Str& joy_name, int hat, HatDirection::Type dir, bool exclusive);
}

namespace Action {


namespace ActionType
{
	enum Type
	{
		Key,
		Mouse,
		JoyButton,
		JoyHat
	};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Action: public Obj
{
	PIXOBJ("Action")

public:
	
	ActionType::Type type;
	// Action type

	Str name;
	// Action Name


	Action(ActionType::Type type, const Str& name)
	{
		this->type = type;
		this->name = name;
	}

	~Action()
	{
	}

	virtual bool is_down() { return false; };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class KeyAction: public Action
{
	PIXOBJ("KeyAction")

public:

	KeyCode key;

	KeyAction(const Str& name, KeyCode key) : Action(ActionType::Key, name)
	{
		this->key = key;
	}

	bool is_down()
	{
		return Input::get_keyboard()[key];
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MouseAction: public Action
{
	PIXOBJ("MouseAction")

public:

	int but;

	MouseAction(const Str& name, int but) : Action(ActionType::Mouse, name)
	{
		this->but = but;
	}

	bool is_down()
	{
		if(but<1 || but>32)
			return false;

		return Input::get_mouse()[but];
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class JoyAction: public Action
{
	PIXOBJ("JoyAction")

public:

	Str joy_name;

	JoyAction(ActionType::Type type, const Str& name, const Str& joy_name) : Action(type, name)
	{
		this->joy_name = joy_name;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class JoyButtonAction: public JoyAction
{
	PIXOBJ("JoyButtonAction")

public:

	int but;

	JoyButtonAction(const Str& name, const Str& joy_name, int but) : JoyAction(ActionType::JoyButton, name, joy_name)
	{
		this->but = but;
	}

	bool is_down()
	{
		// Ask the input module if the given button is down
		return Input::is_joy_button_down(joy_name, but);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class JoyHatAction: public JoyAction
{
	PIXOBJ("JoyHatAction")

public:

	int hat;
	HatDirection::Type dir;
	bool exclusive;

	JoyHatAction(const Str& name, const Str& joy_name, int hat, HatDirection::Type dir, bool exclusive) : JoyAction(ActionType::JoyHat, name, joy_name)
	{
		this->hat = hat;
		this->dir = dir;
		this->exclusive = exclusive;
	}

	bool is_down()
	{
		// Ask the input module if the hat direction is considered active
		return Input::is_joy_hat(joy_name, hat, dir, exclusive);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






class Section: public Obj
{
	PIXOBJ("Section")

public:

	Str name;
	// Section name;

	bool active;
	// Wether the section is active

	List<Action*> actions;
	// All the actions in the section


	Section()
	{
	}

	~Section()
	{
		actions.clear_del();
	}
};



static List<Section*> _sections;
// All the created sections



void init()
{
}

void done()
{
	// Clear all the mappings
	_sections.clear_del();
}


static Section *get_section(const Str& name)
{
	// Find the section with the given name
	for(int c = 0; c<_sections.size(); c++)
		if(_sections[c]->name==name)
			return _sections[c];

	// Not found, create a new one
	Section *sec = new Section();
	sec->name = name;
	sec->active = false;

	_sections.add(sec);

	return sec;
}

void map_key(const Str& section, const Str& action, KeyCode code)
{
	// Add a key mapping
	Section *sec = get_section(section);
	sec->actions.add(new KeyAction(action, code));
}

void map_mouse_button(const Str& section, const Str& action, int button)
{
	// Add a mouse button mapping
	Section *sec = get_section(section);
	sec->actions.add(new MouseAction(action, button));
}

void map_joy_button(const Str& section, const Str& action, const Str& joy_name, int button)
{
	// Add a joystick button mapping
	Section *sec = get_section(section);
	sec->actions.add(new JoyButtonAction(action, joy_name, button));
}

void map_joy_hat(const Str& section, const Str& action, const Str& joy_name, int hat, HatDirection::Type dir, bool exclusive)
{
	// Add a joystick hat mapping
	Section *sec = get_section(section);
	sec->actions.add(new JoyHatAction(action, joy_name, hat, dir, exclusive));
}




static void get_actions(ActionType::Type type, List<Action*>& acts)
{
	// Check all sections
	for(int c = 0; c<_sections.size(); c++)
	{
		Section *sec = _sections[c];

		if(!sec->active)
			continue;

		// Check the actions
		for(int d = 0; d<sec->actions.size(); d++)
		{
			Action *act = sec->actions[d];

			// Add it to the list if it's the proper type
			if(act->type==type)
				acts.add(act);
		}
	}
}

void trigger_key(EventHandler *eh, KeyCode key)
{
	// Get all the key actions
	List<Action*> acts;
	get_actions(ActionType::Key, acts);

	for(int c = 0; c<acts.size(); c++)
	{
		KeyAction *act = (KeyAction*)acts[c];

		if(act->key==key)
			eh->on_action(act->name);
	}

	acts.clear_nodel();
}

void trigger_mouse(EventHandler *eh, int but)
{
	// Get all the mouse actions
	List<Action*> acts;
	get_actions(ActionType::Mouse, acts);

	for(int c = 0; c<acts.size(); c++)
	{
		MouseAction *act = (MouseAction*)acts[c];

		if(act->but==but)
			eh->on_action(act->name);
	}

	acts.clear_nodel();
}

void trigger_joy_button(EventHandler *eh, const Str& joy_name, int but)
{
	// Get all the joy button actions
	List<Action*> acts;
	get_actions(ActionType::JoyButton, acts);

	for(int c = 0; c<acts.size(); c++)
	{
		JoyButtonAction *act = (JoyButtonAction*)acts[c];

		if(act->joy_name==joy_name && act->but==but)
			eh->on_action(act->name);
	}

	acts.clear_nodel();
}

void trigger_joy_hat(EventHandler *eh, const Str& joy_name, int hat, HatDirection::Type dir)
{
	// Get all the joy hat actions
	List<Action*> acts;
	get_actions(ActionType::JoyHat, acts);

	for(int c = 0; c<acts.size(); c++)
	{
		JoyHatAction *act = (JoyHatAction*)acts[c];

		if(act->joy_name==joy_name && act->hat==hat)
		{
			// Hats are a tiny bit more involved
			if(act->dir==dir)
			{
				// Exact direction
				eh->on_action(act->name);
			}
			else if(!act->exclusive && Input::is_hat_sub(dir, act->dir))
			{
				// Sub-direction
				eh->on_action(act->name);
			}
		}
	}

	acts.clear_nodel();
}




void enable(const Str& section, bool enable)
{
	// Enable/disable a section
	get_section(section)->active = enable;
}

bool is_down(const Str& action)
{
	// Check all sections
	for(int c = 0; c<_sections.size(); c++)
	{
		Section *sec = _sections[c];

		if(!sec->active)
			continue;

		// Check the actions
		for(int d = 0; d<sec->actions.size(); d++)
		{
			Action *act = sec->actions[d];

			if(act->name==action && act->is_down())
				return true;
		}
	}

	return false;
}

}}
