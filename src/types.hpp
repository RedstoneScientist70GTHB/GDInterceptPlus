#pragma once

// Shared types and presets. Original code for GDIntercept Plus.

#include <string>
#include <vector>

namespace gdip {

struct Draft {
    std::string method = "GET";
    std::string url;
    std::string headers; // "Name: value" entries separated by a literal backslash-n
    std::string body;
};

struct QuickLink {
    const char* label;
    const char* url;
    const char* method;
    const char* headers;
};

// "Auto links": one click fills the URL (and sensible method/headers).
// URLs only - nothing here pulls credentials, tokens or secret keys.
inline const std::vector<QuickLink>& quickLinks() {
    static const std::vector<QuickLink> links = {
        {"httpbin echo (safe test)", "https://httpbin.org/anything", "POST", ""},
        {"GD server (add an endpoint)", "https://www.boomlings.com/database/", "POST", "Content-Type: application/x-www-form-urlencoded"},
        {"Geode API: mods", "https://api.geode-sdk.org/v1/mods", "GET", ""},
        {"Geode API: loader versions", "https://api.geode-sdk.org/v1/loader/versions", "GET", ""},
    };
    return links;
}

inline const std::vector<std::string>& methods() {
    static const std::vector<std::string> m = {"GET", "POST", "PUT", "PATCH", "DELETE", "HEAD"};
    return m;
}

inline bool methodHasBody(std::string const& method) {
    return method != "GET" && method != "HEAD";
}

} // namespace gdip
