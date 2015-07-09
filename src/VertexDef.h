#pragma once


#define MAX_VERTEX_COMPONENTS		32
// Maximum components in a definition


class VertexDef
{
	List<const VertexComp*> _comps;
	// The list of all components defined by this definition

	void copy(const VertexDef& o);

public:

	VertexDef();
	VertexDef(const VertexDef& o) { copy(o); }
	~VertexDef();

	VertexDef& operator=(const VertexDef& o )
	{
		copy(o);
		return *this;
	}

	VertexDef& operator=(const VertexDef *o )
	{
		copy(*o);
		return *this;
	}

	void clear();
	// Clear all the definition so far

	void add(const VertexComp& o);
	// Add a pre-defined component

	void add(VertexComp *o) { add(*o); }
	// Add a pre-defined component, a copy is made

	int validate() const;
	// Validate the vertex definition
	// If any problem is found, an E::BadVertexComp or E::BadVertexDef error is thrown
	// Upon success, the function returns the minimum stride required to hold such a vertex
	// The minimum stride is the largest component offset plus its size

	int num_comp() const { return _comps.size(); }
	// Return the number of components

	const VertexComp *get_comp(int index) const { return _comps.get(index); }
	// Return a component
};


