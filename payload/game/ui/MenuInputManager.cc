#include "MenuInputManager.hh"

namespace UI {

MenuInputManager::~MenuInputManager() {
    dt(-1);
}

PageInputManager::~PageInputManager() {
    dt(-1);
}

MultiControlInputManager::~MultiControlInputManager() {
    dt(-1);
}

u32 MultiControlInputManager::sequenceFlags(u32 localPlayerId) const {
    return m_players[localPlayerId].sequenceFlags();
}

u32 MultiControlInputManager::Player::sequenceFlags() const {
    return m_sequenceFlags;
}

} // namespace UI
