#pragma once

#include "game/ui/CourseSelectButton.hh"

namespace UI {

class CourseSelectControl : public LayoutUIControl {
public:
    CourseSelectControl();
    ~CourseSelectControl() override;

    void calcSelf() override;

    void load();

    void selectDefault(u32 localPlayerId);

    bool isHidden() const;
    bool isShown() const;

    void hide();
    void slideInFromLeft();
    void slideInFromRight();

    void show();
    void slideOutToLeft();
    void slideOutToRight();

private:
    bool hasSlidIn() const;
    void onHide();
    void onShow();

    std::array<CourseSelectButton, 9> m_buttons;
};

} // namespace UI
