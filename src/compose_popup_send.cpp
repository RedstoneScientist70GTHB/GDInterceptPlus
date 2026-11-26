#include "compose_popup.hpp"

#include <chrono>

void ComposePopup::onSend(CCObject*) {
    std::string url = gdip::normalizeUrl(m_url->getString());
    if (url.empty()) {
        this->setStatus("Enter a URL first.", {255, 170, 120});
        return;
    }
    m_url->setString(url, false);
    std::string method = this->currentMethod();

    web::WebRequest req;
    req.timeout(std::chrono::seconds(Mod::get()->getSettingValue<int64_t>("timeout")));

    auto headers = gdip::parseHeaders(m_headers->getString());
    for (auto const& [name, value] : headers) {
        req.header(name, value);
    }

    std::string body = m_body->getString();
    if (gdip::methodHasBody(method) && !body.empty()) {
        if (!gdip::hasHeader(headers, "Content-Type")) {
            req.header("Content-Type", "application/x-www-form-urlencoded");
        }
        req.bodyString(body);
    }

    this->saveDraft();
    this->setStatus(fmt::format("Sending {}...", method));
    m_preview->setString("");
    m_lastResponse.clear();

    auto started = std::chrono::steady_clock::now();
    m_holder.spawn(req.send(method, url), [this, started, method](web::WebResponse res) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - started
        ).count();
        int code = res.code();

        if (code <= 0) {
            this->setStatus(fmt::format("{} - no response ({} ms). Network error or timeout.", method, ms), {255, 120, 120});
            return;
        }

        ccColor3B color = {140, 255, 160};
        if (code >= 400) color = {255, 120, 120};
        else if (code >= 300) color = {255, 230, 120};

        m_lastResponse = res.string().unwrapOr(std::string("(the body is not valid text)"));
        auto chars = static_cast<size_t>(Mod::get()->getSettingValue<int64_t>("preview-chars"));
        m_preview->setString(gdip::previewText(m_lastResponse, chars, 6).c_str());
        this->setStatus(fmt::format("{} {} - {} ms - {} bytes", method, code, ms, m_lastResponse.size()), color);
    });
}
