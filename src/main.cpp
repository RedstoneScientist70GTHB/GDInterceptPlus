#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/utils/cocos.hpp>

#include "compose_popup.hpp"

using namespace geode::prelude;

// ===========================================================================
// "+" button inside GDIntercept's window
//
// GDIntercept's capture window is a popup with the node ID below. We don't
// touch its code - when that popup is shown we just add a button to its menu.
// If that ID ever changes the button simply won't appear; the keybind still works.
// ===========================================================================
class $modify(PlusAlertLayer, FLAlertLayer) {
    void show() {
        FLAlertLayer::show();
        if (this->getID() == "smjs.gdintercept/intercept_popup") {
            this->addPlusButton();
        }
    }

    void addPlusButton() {
        if (!m_buttonMenu || m_buttonMenu->getChildByID("plus-button"_spr)) return;

        // Sit just left of GDIntercept's top-right button (its settings gear).
        CCNode* rightmost = nullptr;
        for (auto* child : CCArrayExt<CCNode*>(m_buttonMenu->getChildren())) {
            if (!typeinfo_cast<CCMenuItem*>(child)) continue;
            if (!rightmost || child->getPositionX() > rightmost->getPositionX()) rightmost = child;
        }

        auto* sprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        sprite->setScale(0.42f);
        auto* button = CCMenuItemExt::createSpriteExtra(sprite, [](CCMenuItemSpriteExtra*) {
            ComposePopup::open();
        });
        button->setID("plus-button"_spr);

        CCPoint pos;
        if (rightmost) {
            pos.x = rightmost->getPositionX() - rightmost->getScaledContentWidth() / 2.f - button->getScaledContentWidth() / 2.f - 6.f;
            pos.y = rightmost->getPositionY();
        } else {
            pos = {m_mainLayer->getContentWidth() - 40.f, m_mainLayer->getContentHeight() - 18.f};
        }
        button->setPosition(pos);
        m_buttonMenu->addChild(button);
    }
};

// ============================================================================
// Keybind: open the composer from anywhere
// ===========================================================================
$execute {
    listenForKeybindSettingPresses("open_composer", [](Keybind const&, bool down, bool repeat, double) {
        if (down && !repeat) ComposePopup::open();
  });
}
