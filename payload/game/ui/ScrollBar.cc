#include "ScrollBar.hh"

#include "game/ui/Page.hh"

#include <cmath>

namespace UI {

namespace GroupId {

enum {
    Loop = 0,
    Select = 1,
    SelectIn = 2,
    OK = 3,
};

} // namespace GroupId

namespace AnimId::Select {

enum {
    Free = 0,
    FreeToSelect = 1,
    Select = 2 ,
    SelectToFree = 3,
};

} // namespace AnimId::Select

ScrollBar::ScrollBar() = default;

ScrollBar::~ScrollBar() = default;

void ScrollBar::initSelf() {
    m_animator.setAnimationInactive(2, 1, 0.0f);
    m_animator.setAnimationInactive(3, 1, 0.0f);
}

void ScrollBar::calcSelf() {
    bool loopActive = m_animator.getAnimation(GroupId::Select) != AnimId::Select::Free;
    m_animator.setActive(GroupId::Loop, loopActive);

    auto *fuchiPatternMaterial = m_fuchiPatternPane->getMaterial();
    assert(fuchiPatternMaterial);
    m_inputManager.getFuchiPatternColors(&fuchiPatternMaterial->tevColors[0],
            &fuchiPatternMaterial->tevColors[1]);

    GXColorS10 colorBaseColor;
    m_inputManager.getColorBaseColor(&colorBaseColor);
    auto *colorBaseMaterial = m_colorBasePane->getMaterial();
    assert(colorBaseMaterial);
    colorBaseMaterial->tevColors[0] = colorBaseColor;
    colorBaseMaterial->tevColors[1] = colorBaseColor;

    if (m_sequence) {
        auto *parentInputManager = getPage()->inputManager()->downcast<MultiControlInputManager>();
        assert(parentInputManager);
        u32 flags = parentInputManager->sequenceFlags(m_sequence->localPlayerId);
        flags &= 1 << 3 | 1 << 2;
        if (m_sequence->flags == 0 || flags == m_sequence->flags) {
            if (m_sequence->frames % 5 == 0) {
                if (flags & 1 << 2) {
                    if (m_chosen != m_count - 1) {
                        m_chosen++;
                        if (m_changeHandler) {
                            m_changeHandler->handle(this, m_sequence->localPlayerId, m_chosen);
                        }
                    }
                } else if (flags & 1 << 3) {
                    if (m_chosen != 0) {
                        m_chosen--;
                        if (m_changeHandler) {
                            m_changeHandler->handle(this, m_sequence->localPlayerId, m_chosen);
                        }
                    }
                }
            }
            m_sequence->frames++;
        } else {
            m_sequence->frames = 0;
        }
        m_sequence->flags = flags;
    }

    m_thumbFuchiPane->m_trans.x = static_cast<s32>((m_colorBasePane->m_width - 4.0f) * 
            ((m_chosen + 0.5f) / m_count - 0.5f));
    m_thumbPane->m_trans.x = m_thumbFuchiPane->m_trans.x;
}

void ScrollBar::load(u32 count, u32 chosen, const char *dir, const char *file, const char *variant,
        u32 playerFlags, bool isMultiPlayer, bool pointerOnly) {
    const char *groups[] = {
        "Loop", "Loop", nullptr,
        "Select", "Free", "FreeToSelect", "Select", "SelectToFree", nullptr,
        "SelectIn", "SelectIn", "SelectStop", nullptr,
        "OK", "OK", "OKStop", nullptr,
        nullptr,
    };
    LayoutUIControl::load(dir, file, variant, groups);

    m_inputManager.m_pane.pane = m_mainLayout.findPaneByName("touch");
    assert(m_inputManager.m_pane.pane);
    m_inputManager.m_pane.selectHandler = &m_onSelect;
    m_inputManager.m_pane.deselectHandler = &m_onDeselect;
    m_inputManager.m_playerFlags = playerFlags;
    m_inputManager.setHandler(MenuInputManager::InputId::Right, &m_onRight, false);
    m_inputManager.setHandler(MenuInputManager::InputId::Left, &m_onLeft, false);
    m_inputManager.init(0, isMultiPlayer, pointerOnly);
    auto *parentInputManager = getPage()->inputManager()->downcast<MultiControlInputManager>();
    assert(parentInputManager);
    parentInputManager->append(&m_inputManager);

    m_fuchiPatternPane = m_mainLayout.findPaneByName("fuchi_pattern");
    assert(m_fuchiPatternPane);
    m_colorBasePane = m_mainLayout.findPaneByName("color_base");
    assert(m_colorBasePane);
    m_thumbFuchiPane = m_mainLayout.findPaneByName("thumb_fuchi");
    assert(m_thumbFuchiPane);
    m_thumbPane = m_mainLayout.findPaneByName("thumb");
    assert(m_thumbPane);

    m_animator.setAnimationInactive(0, 0, 0.0f);
    m_animator.setAnimationInactive(1, 0, 0.0f);

    m_count = count;
    m_chosen = chosen;

    m_thumbFuchiPane->m_width = std::max((m_colorBasePane->m_width - 4.0f) / m_count, 5.0f);
    m_thumbPane->m_width = m_thumbFuchiPane->m_width - 4.5f;
}

void ScrollBar::setChangeHandler(IHandler *handler) {
    m_changeHandler = handler;
}

void ScrollBar::onSelect([[maybe_unused]] u32 localPlayerId) {
    auto *group = m_animator.getGroup(GroupId::Select);
    if (group->getAnimation() == AnimId::Select::Free) {
        group->setAnimation(AnimId::Select::FreeToSelect, 0.0f);
    } else if (group->getAnimation() == AnimId::Select::SelectToFree) {
        f32 frame = (1.0f - group->getFrame() / group->getDuration(AnimId::Select::SelectToFree)) *
                group->getDuration(AnimId::Select::FreeToSelect);
        group->setAnimation(AnimId::Select::FreeToSelect, frame);
    }

    auto *parentInputManager = getPage()->inputManager()->downcast<MultiControlInputManager>();
    assert(parentInputManager);
    u32 flags = parentInputManager->sequenceFlags(localPlayerId);
    flags &= 1 << 3 | 1 << 2;
    m_sequence.emplace(localPlayerId, flags, 0);
}

void ScrollBar::onDeselect([[maybe_unused]] u32 localPlayerId) {
    auto *group = m_animator.getGroup(GroupId::Select);
    if (group->getAnimation() == AnimId::Select::Select) {
        group->setAnimation(AnimId::Select::SelectToFree, 0.0f);
    } else if (group->getAnimation() == AnimId::Select::FreeToSelect) {
        f32 frame = (1.0f - group->getFrame() / group->getDuration(AnimId::Select::FreeToSelect)) *
                group->getDuration(AnimId::Select::SelectToFree);
        group->setAnimation(AnimId::Select::SelectToFree, frame);
    }

    m_sequence.reset();
}

void ScrollBar::onRight([[maybe_unused]] u32 localPlayerId) {}

void ScrollBar::onLeft([[maybe_unused]] u32 localPlayerId) {}

} // namespace UI
