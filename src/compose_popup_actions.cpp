#include "compose_popup.hpp"

void ComposePopup::onMethod(CCObject*) {
    this->setMethod(gdip::methods()[(m_methodIndex + 1) % gdip::methods().size()]);
    this->saveDraft();
}

// "Auto links": one click fills in the URL (and method/headers) for a common
// target. Only URLs - nothing that touches your login or secrets.
void ComposePopup::onQuickLink(CCObject*) {
    auto const& links = gdip::quickLinks();
    auto const& link = links[m_linkIndex % links.size()];
    ++m_linkIndex;
    m_url->setString(link.url, false);
    this->setMethod(link.method);
    m_headers->setString(link.headers, false);
    this->saveDraft();
    this->setStatus(fmt::format("Quick link: {}", link.label));
}

// Paste a curl command (for example GDIntercept's "Copy Curl") or a plain link.
void ComposePopup::onPasteCurl(CCObject*) {
    std::string clip = geode::utils::clipboard::read();
    gdip::Draft d;
    if (gdip::parseCurl(clip, d)) {
        this->applyDraft(d);
        this->saveDraft();
        this->setStatus("Imported the curl command.", {140, 255, 160});
        return;
    }
    std::string trimmed = gdip::trim(clip);
    if (trimmed.rfind("http", 0) == 0 && trimmed.find(' ') == std::string::npos) {
        m_url->setString(trimmed, false);
        this->saveDraft();
        this->setStatus("Pasted the link.", {140, 255, 160});
        return;
    }
    this->setStatus("The clipboard has no curl command or link.", {255, 170, 120});
}

void ComposePopup::onCopy(CCObject*) {
    if (m_lastResponse.empty()) {
        this->setStatus("No response to copy yet.", {255, 170, 120});
        return;
    }
    geode::utils::clipboard::write(m_lastResponse);
    this->setStatus("Copied the full response.", {140, 255, 160});
}

void ComposePopup::onClear(CCObject*) {
    m_url->setString("", false);
    m_headers->setString("", false);
    m_body->setString("", false);
    this->setMethod("GET");
    m_lastResponse.clear();
    m_preview->setString("");
    this->saveDraft();
    this->setStatus("Cleared.");
}
