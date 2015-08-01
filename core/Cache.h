#pragma once



namespace Cache
{


void add(const Str& name, void *ptr, void (*kill)(const Str&, void*));
// Add an item to the cache

void *get(const Str& name);
// Retrieve an item from the cache
// If not found, NULL is returned

void remove(const Str& name);
// Remove an item from the cache
// Does nothing if not found

void clear();
// Clear the cache


}
