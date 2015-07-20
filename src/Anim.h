#pragma once


class AnimFrame
{
public:

	Texture *tex;
	// Texture to take the frame from

	int w, h;
	// Frame size

	float u1, v1, u2, v2;
	// Texture coordinates for the frame

	int frames;
	// Time the frame will be displayed
};

class AnimSet
{
public:

	Str _name;
	// Set name

	List<AnimFrame*> _frames;


	AnimSet(const Str& name)
	{
		_name = name;
	}

	~AnimSet()
	{
		_frames.clear_del();
	}
};



class Anim
{
	List<AnimSet*> _sets;
	// All the sets

	AnimSet *_cur_set;
	// Current set

	int _cur_frame;
	// Current frame index within the set

	int _frames;
	// Frames counter, to know when its time to change


	AnimSet *get_set(const Str& name, bool add);
	// Get a set

public:

	Anim();
	~Anim();

	//
	// Functions to add frames
	// All these functions take various parameters to add frames to a set,
	// but all have 3 common parameters:
	// 'set_name' is the name of the set to add the frame(s) to.  If the
	// set doesn't exist, it will be created
	// 'tex' is the source texture.  A copy of the texture is NOT made, it
	// is only references, so the texture should be kept alive
	// 'frames' is the number of frames that a specific frame will be visible
	// for.  If the function addes several frames, the same value will be
	// applied to all of them
	// All functions return the index of the first added frame within the set
	//
	
	int add_frame(const Str& set_name, Texture *tex, int frames);
	// Use the full texture

	int add_frame(const Str& set_name, Texture *tex, int frames, int x, int y, int w, int h);
	// Add a sub-section of the texture as the frame

	int add_frame(const Str& set_name, Texture *tex, int frames, int hdiv, int vdiv);
	// Split the texture into (hdiv x vdiv) sub-frames and add them from top-left, horizontally, then vertically

	int add_frame(const Str& set_name, Texture *tex, int frames, int x, int y, int w, int h, int hdiv, int vdiv);
	// Split a sub-section of the texture into (hdiv x vdiv) sub-frames and add them from top-left, horizontally, then vertically

	//
	//
	//

	void set_set(const Str& name, int frame = 0);
	// Change the current set and starting frame index
	// The set must exist, or an error will be thrown

	void change_set(const Str& name, int frame = 0);
	// Change the current set and starting frame index
	// The set must exist, or an error will be thrown
	// If the given set name is the current set, nothing will happen

	AnimFrame *get_frame(bool advance = true);
	// Get the current frame that should be displayed
	// If 'advance' is true, calculations will be made and frames will advance as needed
};


