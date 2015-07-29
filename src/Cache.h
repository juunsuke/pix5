#pragma once

namespace Cache
{


void flush();
// Flush all the cache


Texture *texture(const Str& fname, bool try_load = true);
// Load and cache a texture
// If 'try_load' is false, and the texture isn't currently cached,
// a load attempt will not be done, and NULL will be returned

void texture(const Str& name, Texture *tex);
// Manually add a texture to the cache, with the given name

bool has_texture(const Str& name);
// Check wether the cache contains a texture with the given name

Font *font(const Str& fname, int ptsize);
Font *font(const uint8_t *data, uint32_t len, int ptsize);
// Load and cache a font



}
