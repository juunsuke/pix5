#pragma once

namespace Cache
{


void flush();
// Flush all the cache


Texture *texture(const Str& fname);
// Load and cache a texture

Font *font(const Str& fname, int ptsize);
Font *font(const uint8_t *data, uint32_t len, int ptsize);
// Load and cache a font



}
