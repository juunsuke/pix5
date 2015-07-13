#pragma once

namespace Cache
{


void flush();
// Flush all the cache


Texture *texture(const Str& fname);
// Load and cache a texture


}
