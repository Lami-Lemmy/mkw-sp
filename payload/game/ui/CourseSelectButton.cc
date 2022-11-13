#include "CourseSelectButton.hh"

#include <sp/storage/Storage.hh>

namespace UI {

CourseSelectButton::CourseSelectButton() : m_tpl(new (nullptr, 0x20) u8[138688]) {}

CourseSelectButton::~CourseSelectButton() = default;

void CourseSelectButton::load(u32 i) {
    char variant[0x20];
    snprintf(variant, std::size(variant), "Button%zu", i);
    PushButton::load("button", "CourseSelectButton", variant, 0x1, false, false);
    m_index = i;
}

void CourseSelectButton::refresh(u32 courseIndex) {
    setMessageAll(9360 + courseIndex);

    wchar_t path[0x20];
    swprintf(path, std::size(path), L"/mkw-sp/courses/%u.tpl", courseIndex);
    SP::Storage::ReadFile(path, m_tpl.get(), 138688);

    auto *pane = m_mainLayout.findPaneByName("picture_base");
    assert(pane);
    auto *material = pane->getMaterial();
    assert(material);
    auto *texMap = material->getTexMapAry();
    assert(texMap);
    texMap->replaceImage(m_tpl.get(), 0);
}

} // namespace UI

