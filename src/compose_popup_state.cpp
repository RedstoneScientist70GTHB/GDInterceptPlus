#include "compose_popup.hpp"

std::string ComposePopup::currentMethod() const {
    return gdip::methods()[m_methodIndex % gdip::methods().size()];
}

void ComposePopup::setMethod(std::string const& method) {
    auto const& all = gdip::methods();
    auto it = std::find(all.begin(), all.end(), gdip::toUpper(method));
    m_methodIndex = (it == all.end()) ? 0 : static_cast<size_t>(it - all.begin());
    m_methodSpr->setString(all[m_methodIndex].c_str());
}

void ComposePopup::setStatus(std::string const& text, ccColor3B color) {
    m_status->setString(text.c_str());
    m_status->setColor(color);
}

void ComposePopup::applyDraft(gdip::Draft const& d) {
    this->setMethod(d.method);
    m_url->setString(d.url, false);
    m_headers->setString(d.headers, false);
    m_body->setString(d.body, false);
}

void ComposePopup::saveDraft() {
    auto* mod = Mod::get();
    mod->setSavedValue<std::string>("draft-method", this->currentMethod());
    mod->setSavedValue<std::string>("draft-url", m_url->getString());
    mod->setSavedValue<std::string>("draft-headers", m_headers->getString());
    mod->setSavedValue<std::string>("draft-body", m_body->getString());
}

void ComposePopup::loadDraft() {
    auto* mod = Mod::get();
    gdip::Draft d;
    d.method = mod->getSavedValue<std::string>("draft-method", "GET");
    d.url = mod->getSavedValue<std::string>("draft-url", "");
    d.headers = mod->getSavedValue<std::string>("draft-headers", "");
    d.body = mod->getSavedValue<std::string>("draft-body", "");
    this->applyDraft(d);
}

ComposePopup* ComposePopup::create() {
    auto ret = new ComposePopup();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void ComposePopup::open() {
    auto* scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene || scene->getChildByID("composer_popup"_spr)) return;
    if (auto* popup = ComposePopup::create()) {
        popup->setID("composer_popup"_spr);
        popup->show();
    }
}
