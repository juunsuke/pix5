#include "pix5.h"

namespace PIX {




Anim::Anim()
{
}

Anim::~Anim()
{
	_sets.clear_del();
}

int Anim::get_set(const Str& name, bool add)
{
	// Find the set
	for(int c = 0; c<_sets.size(); c++)
		if(_sets[c]->_name==name)
			return c;

	// Doesn't exist, create it  ?
	if(!add)
		// No
		return -1;

	// Yes
	AnimSet *set = new AnimSet(name);
	return _sets.add(set);
}

int Anim::add_frame(const Str& set_name, Texture *tex, int frames)
{
	return add_frame(set_name, tex, frames, 0, 0, tex->width(), tex->height());
}

int Anim::add_frame(const Str& set_name, Texture *tex, int frames, int x, int y, int w, int h)
{
	ASSERT(w>0 && h>0, "Anim::add_frame(): Invalid width/height")

	// Get/create the set
	AnimSet *set = _sets[get_set(set_name, true)];

	// Add the frame to it
	AnimFrame *frame = new AnimFrame();
	frame->tex = tex;
	frame->frames = frames;

	frame->w = w;
	frame->h = h;

	float uf = 1.0f / (float)tex->width();
	float vf = 1.0f / (float)tex->height();

	frame->u1 = (float)x*uf;
	frame->v1 = (float)y*vf;
	frame->u2 = (float)(x+w)*uf;
	frame->v2 = (float)(y+h)*vf;

	// Add it
	return set->_frames.add(frame);
}

int Anim::add_frame(const Str& set_name, Texture *tex, int frames, int hdiv, int vdiv)
{
	return add_frame(set_name, tex, frames, 0, 0, tex->width(), tex->height(), hdiv, vdiv);
}

int Anim::add_frame(const Str& set_name, Texture *tex, int frames, int x, int y, int w, int h, int hdiv, int vdiv)
{
	ASSERT(hdiv>0 && vdiv>0, "Anim::add_frame(): Invalid h/v sub-division factor")
	ASSERT((w%hdiv)==0, "Anim::add_frame(): Width is not divisible by hdiv")
	ASSERT((h%vdiv)==0, "Anim::add_frame(): Height is not divisible by vdiv")

	// Add all the sub-frames
	int fw = w/hdiv;
	int fh = h/vdiv;

	for(int cy = 0; cy<vdiv; cy++)
		for(int cx = 0; cx<hdiv; cx++)
			add_frame(set_name, tex, frames, x+cx*fw, y+cy*fw, fw, fh);

	// Return the index of the first frame
	return _sets[get_set(set_name, false)]->_frames.size()-(fw*fh);
}

void Anim::set_set(AnimState &state, const Str& name, int frame)
{
	// Change the current set
	int set = get_set(name, false);
	if(set==-1)
		E::BadAnimSet("Anim::set_set(): Unknown set: '%s'", name.ptr());

	// Adjust frame properly
	if(frame<0 || frame>=_sets[set]->_frames.size())
		frame = 0;

	// Make it active
	state.cur_set = set;
	state.cur_frame = frame;
	state.counter = 0;
}
	
void Anim::change_set(AnimState &state, const Str& name, int frame)
{
	// Do nothing if the current set is the given one
	int set = get_set(name, false);
	if(set==-1)
		E::BadAnimSet("Anim::set_set(): Unknown set: '%s'", name.ptr());

	// Change the set if it changed
	if(set!=state.cur_set)
	{
		// Adjust frame properly
		if(frame<0 || frame>=_sets[set]->_frames.size())
			frame = 0;

		// Make it active
		state.cur_set = set;
		state.cur_frame = frame;
		state.counter = 0;
	}
}

AnimFrame *Anim::get_frame(AnimState &state, bool advance)
{
	if(_sets.size()==0)
		E::BadAnimSet("Anim::get_frame(): No defined set");
	if(state.cur_set<0 || state.cur_set>=_sets.size())
		E::BadAnimSet("Anim::get_frame(): Invalid state set");

	// Get the current frame
	AnimSet *set = _sets[state.cur_set];
	AnimFrame *frm = set->_frames[state.cur_frame];

	// Advance if needed
	if(advance)
	{
		// Advance the counter by 1 frame
		state.counter++;

		// Check if it reached the current frame's limit
		if(state.counter>=frm->frames)
		{
			// Yes, advance to the next frame
			state.cur_frame++;
			if(state.cur_frame>=set->_frames.size())
				state.cur_frame = 0;

			state.counter = 0;
		}
	}

	return frm;
}



}
