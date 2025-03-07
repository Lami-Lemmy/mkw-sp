#include "ModelRenderPage.hh"

#include "game/system/GameScene.hh"
#include "game/system/ResourceManager.hh"
#include "game/ui/SectionManager.hh"
#include "game/ui/model/MenuModelManager.hh"

namespace UI {

void ModelRenderPage::onInit() {
    for (u8 i = 0; i < 4; i++) {
        _54[i]._00 = 0;
        _54[i]._04 = 4;
        _54[i]._08 = 0;
        _54[i]._09 = 0;
    }

    _88 = 0;
    _90 = 0;

    if (!MenuModelManager::Instance()) {
        _90 = 1;
        m_inputManager.init(0, false);
        setInputManager(&m_inputManager);
        initChildren(0);
        return;
    }

    auto sectionId = SectionManager::Instance()->currentSection()->id();
    m_modelCount = determineModelCount(sectionId);

    auto *gameScene = System::GameScene::Instance();
    gameScene->volatileHeapCollection.setGroupIdAll(3);

    System::ResourceManager::Instance()->createMenuHeaps(m_modelCount, 1);
    MenuModelManager::Instance()->init(m_modelCount, &onDriverModelLoaded);

    gameScene->volatileHeapCollection.setGroupIdAll(6);

    m_inputManager.init(0, false);
    setInputManager(&m_inputManager);
    initChildren(0);
}

} // namespace UI
