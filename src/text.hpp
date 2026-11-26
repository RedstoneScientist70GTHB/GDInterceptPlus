#pragma once

// Text helpers: trim, URL and header parsing, response preview.

#include <algorithm>
#include <cctype>
#include <string>
#include <utility>
#include <vector>

namespace gdip {

inline std::string trim(std::string s) {
    auto notSpace = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
    return s;
}

inline std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

inline std::string toUpper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
    return s;
}

// Add https:// when the user typed a bare host. Keeps http:// and https://.
inline std::string normalizeUrl(std::string url) {
    url = trim(std::move(url));
    if (url.empty()) return url;
    std::string lower = toLower(url.substr(0, 8));
    if (lower.rfind("http://", 0) == 0 || lower.rfind("https://", 0) == 0) return url;
    return "https://" + url;
}

// Split "Name: value" entries separated by a literal "\n" (backslash + n) or a
// real newline. Entries without a colon are skipped.
inline std::vector<std::pair<std::string, std::string>> parseHeaders(std::string const& text) {
    std::string norm;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\\' && i + 1 < text.size() && text[i + 1] == 'n') {
            norm += '\n';
            ++i;
        } else {
            norm += text[i];
        }
    }

    std::vector<std::pair<std::string, std::string>> out;
    size_t start = 0;
    while (start <= norm.size()) {
        size_t end = norm.find_first_of("\r\n", start);
        std::string line = norm.substr(start, end == std::string::npos ? std::string::npos : end - start);
        auto colon = line.find(':');
        if (colon != std::string::npos) {
            std::string name = trim(line.substr(0, colon));
            std::string value = trim(line.substr(colon + 1));
            if (!name.empty()) out.emplace_back(std::move(name), std::move(value));
        }
        if (end == std::string::npos) break;
        start = end + 1;
    }
    return out;
}

inline bool hasHeader(std::vector<std::pair<std::string, std::string>> const& headers, std::string const& name) {
    std::string wanted = toLower(name);
    for (auto const& h : headers) {
        if (toLower(h.first) == wanted) return true;
    }
    return false;
}

// Cut long text for on-screen preview: at most maxChars characters and
// maxLines lines, with a note saying how much was hidden.
inline std::string previewText(std::string const& text, size_t maxChars, size_t maxLines) {
    std::string out;
    size_t lines = 1;
    size_t i = 0;
    for (; i < text.size() && out.size() < maxChars; ++i) {
        char c = text[i];
        if (c == '\r') continue;
        if (c == '\t') c = ' ';
        if (c == '\n') {
            if (lines >= maxLines) break;
            ++lines;
        }
        out += c;
    }
    if (i < text.size()) {
        out += "\n... (" + std::to_string(text.size() - i) + " more characters - use Copy)";
    }
    return out;
}

} // namespace gdip
