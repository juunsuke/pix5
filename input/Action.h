#pragma once

namespace Action
{


//
// Action mapping
// Every mapping consist of a section and action name
// A new section will be created automatically the first time an action is mapped within it
// New sections are disabled by default
// Several inputs can be mapped to the same action
// The same input can be mapped to several actions
// Joysticks are mapped using a joystick name, referenced for their axis, buttons and hats
// An ID->Name mapping can be done using map_joystick()
//

void map_key(const Str& section, const Str& action, KeyCode code);
// Map a keyboard key down to an action

void map_mouse_button(const Str& section, const Str& action, int button);
// Map a mouse button to an action
// As a special case, button #33 is wheel up and button #34 is wheel down

void map_joy_button(const Str& section, const Str& action, const Str& joy_name, int button);
// Map a joystick button to an action

void map_joy_hat(const Str& section, const Str& action, const Str& joy_name, int hat, HatDirection::Type dir, bool exclusive = false);
// Map a hat direction to an action
// If 'dir' is not a diagonal (i.e. Up or Left) and 'exclusive' is false, the action will trigger when
// a diagonal direction including the specified direction is triggered




void enable(const Str& section, bool enable = true);
inline static void disable(const Str& section) { enable(section, false); }
// Enable/disable a section

bool is_down(const Str& action);
// Check wether the given action is currently active



}
