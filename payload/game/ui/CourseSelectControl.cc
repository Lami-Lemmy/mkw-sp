#include "CourseSelectControl.hh"

#include "game/ui/CourseSelectPage.hh"
#include "game/ui/SectionManager.hh"

namespace UI {

namespace GroupId {

enum {
    Slide = 0,
};

} // namespace GroupId

namespace AnimId::Slide {

enum {
    Hide = 0,
    SlideInFromLeft = 1,
    SlideInFromRight = 2,
    Show = 3,
    SlideOutToLeft = 4,
    SlideOutToRight = 5,
};

} // namespace AnimId::Slide

CourseSelectControl::CourseSelectControl() = default;

CourseSelectControl::~CourseSelectControl() = default;

void CourseSelectControl::calcSelf() {
    if (hasSlidIn()) {
        for (size_t i = 0; i < m_buttons.size(); ++i) {
            if (m_buttons[i].getVisible()) {
                m_buttons[i].setPlayerFlags(0x1);
            }
        }
        auto *section = SectionManager::Instance()->currentSection();
        auto *courseSelectPage = section->page<PageId::CourseSelect>();
        u32 lastSelected = courseSelectPage->lastSelected();
        if (!m_buttons[lastSelected].getVisible()) {
            lastSelected = 0;
        }
        m_buttons[lastSelected].select(0);

        m_animator.setAnimation(GroupId::Slide, AnimId::Slide::Show, 0.0f);
    }

    setVisible(!isHidden());
}

void CourseSelectControl::load() {
    const char *groups[] = {
        "Slide",
        "Hide",
        "SlideInFromLeft",
        "SlideInFromRight",
        "Show",
        "SlideOutToLeft",
        "SlideOutToRight",
        nullptr,
        nullptr,
    };
    LayoutUIControl::load("control", "CourseSelectBase", "Base", groups);
    m_animator.setAnimation(GroupId::Slide, AnimId::Slide::Hide, 0.0f);

    initChildren(m_buttons.size());
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        insertChild(i, &m_buttons[i]);
        m_buttons[i].load(i);
    }
}

void CourseSelectControl::selectDefault(u32 localPlayerId) {
    m_buttons[0].selectDefault(localPlayerId);
}

bool CourseSelectControl::isHidden() const {
    return m_animator.getAnimation(GroupId::Slide) == AnimId::Slide::Hide;
}

bool CourseSelectControl::isShown() const {
    return m_animator.getAnimation(GroupId::Slide) == AnimId::Slide::Show;
}

void CourseSelectControl::hide() {
    m_animator.setAnimation(GroupId::Slide, AnimId::Slide::Hide, 0.0f);
    onHide();
}

void CourseSelectControl::slideInFromLeft() {
    m_animator.setAnimation(GroupId::Slide, AnimId::Slide::SlideInFromLeft, 0.0f);
    onShow();
}

void CourseSelectControl::slideInFromRight() {
    m_animator.setAnimation(GroupId::Slide, AnimId::Slide::SlideInFromRight, 0.0f);
    onShow();
}

void CourseSelectControl::show() {
    m_animator.setAnimation(GroupId::Slide, AnimId::Slide::Show, 0.0f);
    onShow();
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        u32 flags = m_buttons[i].getVisible() ? 0x1 : 0x0;
        m_buttons[i].setPlayerFlags(flags);
    }
}

void CourseSelectControl::slideOutToLeft() {
    m_animator.setAnimation(GroupId::Slide, AnimId::Slide::SlideOutToLeft, 0.0f);
    onHide();
}

void CourseSelectControl::slideOutToRight() {
    m_animator.setAnimation(GroupId::Slide, AnimId::Slide::SlideOutToRight, 0.0f);
    onHide();
}

bool CourseSelectControl::hasSlidIn() const {
    switch (m_animator.getAnimation(GroupId::Slide)) {
    case AnimId::Slide::SlideInFromLeft:
    case AnimId::Slide::SlideInFromRight:
        return !m_animator.isActive(GroupId::Slide);
    default:
        return false;
    }
}

void CourseSelectControl::onHide() {
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        m_buttons[i].setPlayerFlags(0x0);
    }
}

void CourseSelectControl::onShow() {
    auto *section = SectionManager::Instance()->currentSection();
    auto *courseSelectPage = section->page<PageId::CourseSelect>();
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        u32 courseIndex = courseSelectPage->sheetIndex() * m_buttons.size() + i;
        if (courseIndex < courseSelectPage->courseCount()) {
            m_buttons[i].setVisible(true);
            m_buttons[i].refresh(courseIndex);
        } else {
            m_buttons[i].setVisible(false);
        }
    }
}

} // namespace UI
