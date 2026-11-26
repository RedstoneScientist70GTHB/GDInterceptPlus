#include "compose_popup.hpp"

bool ComposePopup::init() {
    if (!Popup::init(CCSize(440.f, 290.f))) return false;
    this->setTitle("New Request");

    auto addLabel = [this](char const* text, float y) {
        auto label = CCLabelBMFont::create(text, "bigFont.fnt");
        label->setScale(0.4f);
        label->setAnchorPoint({0.f, 0.5f});
        label->setColor({200, 200, 200});
        m_mainLayer->addChildAtPosition(label, Anchor::Center, {-205.f, y});
    };

    // Method button + URL
    m_methodSpr = ButtonSprite::create("GET");
    m_methodSpr->setScale(0.55f);
    auto methodBtn = CCMenuItemSpriteExtra::create(m_methodSpr, this, menu_selector(ComposePopup::onMethod));
    m_buttonMenu->addChildAtPosition(methodBtn, Anchor::Center, {-165.f, 95.f});

    m_url = TextInput::create(300.f, "https://example.com/path", "chatFont.fnt");
    m_url->setCommonFilter(CommonFilter::Any);
    m_url->setMaxCharCount(4000);
    m_mainLayer->addChildAtPosition(m_url, Anchor::Center, {55.f, 95.f});

    // Headers
    addLabel("Headers", 60.f);
    m_headers = TextInput::create(330.f, "Name: value\\nName2: value2", "chatFont.fnt");
    m_headers->setCommonFilter(CommonFilter::Any);
    m_headers->setMaxCharCount(8000);
    m_mainLayer->addChildAtPosition(m_headers, Anchor::Center, {35.f, 60.f});

    // Body
    addLabel("Body", 25.f);
    m_body = TextInput::create(330.f, "key=value&key2=value2", "chatFont.fnt");
    m_body->setCommonFilter(CommonFilter::Any);
    m_body->setMaxCharCount(20000);
    m_mainLayer->addChildAtPosition(m_body, Anchor::Center, {35.f, 25.f});

    // Buttons
    auto addButton = [this](char const* text, float scale, float x, SEL_MenuHandler selector) {
        auto spr = ButtonSprite::create(text);
        spr->setScale(scale);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, selector);
        m_buttonMenu->addChildAtPosition(btn, Anchor::Center, {x, -15.f});
    };
    addButton("Quick link", 0.5f, -155.f, menu_selector(ComposePopup::onQuickLink));
    addButton("Paste curl", 0.5f, -55.f, menu_selector(ComposePopup::onPasteCurl));
    addButton("Send", 0.55f, 35.f, menu_selector(ComposePopup::onSend));
    addButton("Copy", 0.5f, 110.f, menu_selector(ComposePopup::onCopy));
    addButton("Clear", 0.5f, 175.f, menu_selector(ComposePopup::onClear));

    // Status + response preview
    m_status = CCLabelBMFont::create("Ready.", "bigFont.fnt");
    m_status->setScale(0.4f);
    m_status->setAnchorPoint({0.f, 0.5f});
    m_mainLayer->addChildAtPosition(m_status, Anchor::Center, {-205.f, -45.f});

    m_preview = CCLabelBMFont::create("", "chatFont.fnt");
    m_preview->setScale(0.6f);
    m_preview->setAnchorPoint({0.f, 1.f});
    m_preview->setWidth(680.f);
    m_mainLayer->addChildAtPosition(m_preview, Anchor::Center, {-205.f, -58.f});

    this->loadDraft();

    auto save = [this](std::string const&) { this->saveDraft(); };
    m_url->setCallback(save);
    m_headers->setCallback(save);
    m_body->setCallback(save);

    return true;
}
