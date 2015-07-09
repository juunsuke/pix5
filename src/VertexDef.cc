#include "pix5.h"

namespace PIX {




VertexDef::VertexDef()
{
}

VertexDef::~VertexDef()
{
	clear();
}

void VertexDef::copy(const VertexDef& o)
{
	// Clear anything we might have
	clear();

	// Copy each component
	for(int c = 0; c<o._comps.size(); c++)
		_comps.add(new VertexComp(*(o._comps.get(c))));
}

void VertexDef::clear()
{
	_comps.clear_del();
}

void VertexDef::add(const VertexComp& o)
{
	// Add a component
	_comps.add(new VertexComp(o));
}

int VertexDef::validate() const
{
	// Need at least 1 component
	if(_comps.size()==0)
		E::BadVertexDef("VertexDef::validate(): Empty definition");

	// Can't have too many either
	if(_comps.size()>MAX_VERTEX_COMPONENTS)
		E::BadVertexDef("VertexDef::validate(): Too many components");

	// Check for empty names and duplicates
	for(int c = 0; c<_comps.size(); c++)
	{
		const VertexComp *comp = _comps.get(c);

		if(comp->name=="")
			E::BadVertexDef("VertexDef::validate(): A component has no name");

		for(int d = c+1; d<_comps.size(); d++)
			if(comp->name==_comps.get(d)->name)
				E::BadVertexDef("VertexDef::validate(): Duplicate component name: %s", comp->name.ptr());
	}

	// Validate every component type and size, while adding up their size and checking for overlap
	int sizes[MAX_VERTEX_COMPONENTS];
	memset(sizes, 0, sizeof(sizes));

	int max = 0;
	for(int c = 0; c<_comps.size(); c++)
	{
		const VertexComp *comp = _comps.get(c);

		// Validate and get the size
		int s = comp->get_valid_size();
		sizes[c] = s;

		// Validate the offset
		if(comp->offset<0)
			E::BadVertexDef("VertexDef::validate(): Component '%s' has a negative offset", comp->name.ptr());

		// Fix max
		if((comp->offset+s)>max)
			max = comp->offset+s;

		// Check for overlap
		for(int d = 0; d<c; d++)
		{
			const VertexComp *comp2 = _comps.get(d);

			if(comp->offset>=(comp2->offset+sizes[d]))
				continue;
			if((comp->offset+s)<=comp2->offset)
				continue;

			// There is overlap
			E::BadVertexDef("VertexDef::validate(): Components '%s' and '%s' overlap", comp->name.ptr(), comp2->name.ptr());
		}
	}

	// All seems valid, return the minimum stride
	return max;
}


}
