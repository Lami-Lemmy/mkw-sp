#pragma once

#include "game/ui/CourseSelectButton.hh"
#include "game/ui/Page.hh"
#include "game/ui/ScrollBar.hh"
#include "game/ui/SheetSelectControl.hh"
#include "game/ui/ctrl/CtrlMenuBackButton.hh"
#include "game/ui/ctrl/CtrlMenuPageTitleText.hh"

namespace UI {

class CourseSelectPage : public Page {
public:
    CourseSelectPage();
    CourseSelectPage(const CourseSelectPage &) = delete;
    CourseSelectPage(CourseSelectPage &&) = delete;
    ~CourseSelectPage() override;

    PageId getReplacement() override;
    void onInit() override;
    void onActivate() override;

    u32 courseCount() const;
    u32 sheetIndex() const;
    u32 lastSelected() const;

private:
    void onBack(u32 localPlayerId);
    void onButtonSelect(PushButton *button, u32 localPlayerId);
    void onSheetSelectRight(SheetSelectControl *control, u32 localPlayerId);
    void onSheetSelectLeft(SheetSelectControl *control, u32 localPlayerId);
    void onScrollBarChange(ScrollBar *scrollBar, u32 localPlayerId, u32 chosen);
    void onBackButtonFront(PushButton *button, u32 localPlayerId);

    void refresh();

    template <typename T>
    using H = typename T::Handler<CourseSelectPage>;

    MultiControlInputManager m_inputManager;
    CtrlMenuPageTitleText m_pageTitleText;
    std::array<CourseSelectButton, 9> m_buttons;
    SheetSelectControl m_sheetSelect;
    LayoutUIControl m_sheetLabel;
    ScrollBar m_scrollBar;
    CtrlMenuBackButton m_backButton;
    H<MultiControlInputManager> m_onBack{ this, &CourseSelectPage::onBack };
    H<PushButton> m_onButtonSelect{ this, &CourseSelectPage::onButtonSelect };
    H<SheetSelectControl> m_onSheetSelectRight{ this, &CourseSelectPage::onSheetSelectRight };
    H<SheetSelectControl> m_onSheetSelectLeft{ this, &CourseSelectPage::onSheetSelectLeft };
    H<ScrollBar> m_onScrollBarChange{ this, &CourseSelectPage::onScrollBarChange };
    H<PushButton> m_onBackButtonFront{ this, &CourseSelectPage::onBackButtonFront };
    PageId m_replacement;
    u32 m_sheetCount;
    u32 m_sheetIndex;
    u32 m_lastSelected;
};

} // namespace UI
