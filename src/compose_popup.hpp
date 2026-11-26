#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/utils/web.hpp>
#include <string>

#include "parsing.hpp"

using namespace geode::prelude;

// The "New Request" window. Implementation is spread over a few .cpp files:
//   compose_popup_ui.cpp       - building the window
//   compose_popup_state.cpp    - method/status/draft helpers, create/open
//   compose_popup_actions.cpp  - the small buttons
//   compose_popup_send.cpp     - sending the request
class ComposePopup : public Popup {
protected:
    TextInput* m_url = nullptr;
    TextInput* m_headers = nullptr;
    TextInput* m_body = nullptr;
    ButtonSprite* m_methodSpr = nullptr;
    CCLabelBMFont* m_status = nullptr;
    CCLabelBMFont* m_preview = nullptr;
    size_t m_methodIndex = 0;
    size_t m_linkIndex = 0;
    std::string m_lastResponse;
    async::TaskHolder<web::WebResponse> m_holder;

    bool init();

    std::string currentMethod() const;
    void setMethod(std::string const& method);
    void setStatus(std::string const& text, ccColor3B color = {200, 200, 200});
    void applyDraft(gdip::Draft const& d);
    void saveDraft();
    void loadDraft();

    void onMethod(CCObject*);
    void onQuickLink(CCObject*);
    void onPasteCurl(CCObject*);
    void onCopy(CCObject*);
    void onClear(CCObject*);
    void onSend(CCObject*);

public:
    static ComposePopup* create();
    static void open();
};
