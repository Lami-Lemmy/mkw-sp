#include "CourseSelectPage.hh"

namespace UI {

CourseSelectPage::CourseSelectPage() = default;

CourseSelectPage::~CourseSelectPage() = default;

PageId CourseSelectPage::getReplacement() {
    return m_replacement;
}

void CourseSelectPage::onInit() {
    m_sheetCount = (courseCount() + 9 - 1) / 9;
    m_sheetIndex = 0;
    m_lastSelected = 0;

    m_inputManager.init(0x1, false);
    setInputManager(&m_inputManager);
    m_inputManager.setWrappingMode(MultiControlInputManager::WrappingMode::Neither);

    initChildren(5 + m_buttons.size());
    insertChild(0, &m_pageTitleText, 0);
    insertChild(1, &m_sheetSelect, 0);
    insertChild(2, &m_sheetLabel, 0);
    insertChild(3, &m_scrollBar, 0);
    insertChild(4, &m_backButton, 0);
    for (size_t i = 0; i < m_buttons.size(); i++) {
        insertChild(5 + i, &m_buttons[i], 0);
    }

    m_pageTitleText.load(false);
    for (size_t i = 0; i < m_buttons.size(); i++) {
        m_buttons[i].load(i);
    }
    m_sheetSelect.load("button", "CourseSelectArrowRight", "ButtonArrowRight",
            "CourseSelectArrowLeft", "ButtonArrowLeft", 0x1, false, false);
    m_sheetLabel.load("control", "TimeAttackGhostListPageNum", "TimeAttackGhostListPageNum", NULL);
    m_scrollBar.load(m_sheetCount, m_sheetIndex, "button", "CourseSelectScrollBar",
            "CourseSelectScrollBar", 0x1, false, false);
    m_backButton.load("button", "Back", "ButtonBack", 0x1, false, true);

    m_inputManager.setHandler(MenuInputManager::InputId::Back, &m_onBack, false, false);
    for (size_t i = 0; i < m_buttons.size(); i++) {
        m_buttons[i].setSelectHandler(&m_onButtonSelect, false);
    }
    m_sheetSelect.setRightHandler(&m_onSheetSelectRight);
    m_sheetSelect.setLeftHandler(&m_onSheetSelectLeft);
    m_scrollBar.setChangeHandler(&m_onScrollBarChange);
    m_backButton.setFrontHandler(&m_onBackButtonFront, false);

    m_pageTitleText.setMessage(3405);

    m_sheetSelect.setVisible(m_sheetCount > 1);
    m_sheetSelect.setPlayerFlags(m_sheetCount <= 1 ? 0x0 : 0x1);

    refresh();

    m_buttons[0].selectDefault(0);
}

void CourseSelectPage::onActivate() {
    m_replacement = PageId::None;
}

u32 CourseSelectPage::courseCount() const {
    return 256;
}

u32 CourseSelectPage::sheetIndex() const {
    return m_sheetIndex;
}

u32 CourseSelectPage::lastSelected() const {
    return m_lastSelected;
}

void CourseSelectPage::onBack([[maybe_unused]] u32 localPlayerId) {
    m_replacement = PageId::DriftSelect;
    startReplace(Anim::Prev, 0.0f);
}

void CourseSelectPage::onButtonSelect([[maybe_unused]] PushButton *button,
        [[maybe_unused]] u32 localPlayerId) {
    m_lastSelected = button->m_index;
}

void CourseSelectPage::onSheetSelectRight([[maybe_unused]] SheetSelectControl *control,
        [[maybe_unused]] u32 localPlayerId) {
    if (m_sheetIndex == m_sheetCount - 1) {
        m_sheetIndex = 0;
    } else {
        ++m_sheetIndex;
    }

    refresh();

    m_lastSelected = m_lastSelected / 3 * 3 + 0;
    while (!m_buttons[m_lastSelected].getVisible()) {
        --m_lastSelected;
    }
    m_buttons[m_lastSelected].select(0);

    m_scrollBar.m_chosen = m_sheetIndex;
}

void CourseSelectPage::onSheetSelectLeft([[maybe_unused]] SheetSelectControl *control,
        [[maybe_unused]] u32 localPlayerId) {
    if (m_sheetIndex == 0) {
        m_sheetIndex = m_sheetCount - 1;
    } else {
        --m_sheetIndex;
    }

    refresh();

    m_lastSelected = m_lastSelected / 3 * 3 + 2;
    while (!m_buttons[m_lastSelected].getVisible()) {
        --m_lastSelected;
    }
    m_buttons[m_lastSelected].select(0);

    m_scrollBar.m_chosen = m_sheetIndex;
}

void CourseSelectPage::onScrollBarChange([[maybe_unused]] ScrollBar *scrollBar,
        [[maybe_unused]] u32 localPlayerId, [[maybe_unused]] u32 chosen) {
    m_sheetIndex = chosen;

    refresh();
}

void CourseSelectPage::onBackButtonFront([[maybe_unused]] PushButton *button,
        [[maybe_unused]] u32 localPlayerId) {
    m_replacement = PageId::DriftSelect;
    f32 delay = button->getDelay();
    startReplace(Anim::Prev, delay);
}

void CourseSelectPage::refresh() {
    for (size_t i = 0; i < m_buttons.size(); i++) {
        u32 courseIndex = m_sheetIndex * m_buttons.size() + i;
        if (courseIndex < courseCount()) {
            m_buttons[i].setVisible(true);
            m_buttons[i].setPlayerFlags(1 << 0);
            m_buttons[i].refresh(courseIndex % 32);
        } else {
            m_buttons[i].setVisible(false);
            m_buttons[i].setPlayerFlags(0);
        }
    }

    m_sheetLabel.setVisible(m_sheetCount != 0);
    MessageInfo info{};
    info.intVals[0] = m_sheetIndex + 1;
    info.intVals[1] = m_sheetCount;
    m_sheetLabel.setMessageAll(2009, &info);
}

} // namespace UI
