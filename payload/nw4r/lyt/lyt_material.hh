#pragma once

#include "nw4r/lyt/lyt_texMap.hh"

extern "C" {
#include <revolution.h>
}

namespace nw4r::lyt {

class Material {
public:
    TexMap *getTexMapAry();

private:
    u8 _00[0x10 - 0x00];

public:
    GXColorS10 tevColors[3];

private:
    u8 _28[0x5c - 0x28];
};
static_assert(sizeof(Material) == 0x5c);

} // namespace nw4r::lyt
