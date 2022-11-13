#pragma once

#include "Button.hh"

namespace UI {

class CourseSelectButton : public PushButton {
public:
    CourseSelectButton();
    ~CourseSelectButton();

    void load(u32 i);
    void refresh(u32 courseIndex);

private:
    std::unique_ptr<u8[]> m_tpl;
};

} // namespace UI
