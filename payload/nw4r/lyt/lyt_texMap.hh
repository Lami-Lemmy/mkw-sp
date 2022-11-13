#pragma once

#include <Common.hh>

namespace nw4r::lyt {

class TexMap {
public:
    void replaceImage(void *tpl, u32 index);

private:
    u8 _00[0x1c - 0x00];
};
static_assert(sizeof(TexMap) == 0x1c);

} // namespace nw4r::lyt
