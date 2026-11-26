#pragma once

// Splitting a command line into words.

#include <string>
#include <vector>

namespace gdip {

// Minimal shell-style tokenizer: single quotes, double quotes with backslash
// escapes, bare backslash escapes and backslash-newline continuations.
inline std::vector<std::string> shellSplit(std::string const& s) {
    std::vector<std::string> out;
    std::string cur;
    bool inToken = false;
    char quote = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (quote == '\'') {
            if (c == '\'') quote = 0; else cur += c;
            continue;
        }
        if (quote == '"') {
            if (c == '\\' && i + 1 < s.size()) {
                char n = s[i + 1];
                if (n == '"' || n == '\\' || n == '$' || n == '`') { cur += n; ++i; }
                else cur += c;
            } else if (c == '"') {
                quote = 0;
            } else {
                cur += c;
            }
            continue;
        }
        if (c == '\'' || c == '"') { quote = c; inToken = true; continue; }
        if (c == '\\' && i + 1 < s.size()) {
            if (s[i + 1] == '\n' || s[i + 1] == '\r') { ++i; continue; }
            cur += s[i + 1];
            ++i;
            inToken = true;
            continue;
        }
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (inToken) { out.push_back(cur); cur.clear(); inToken = false; }
            continue;
        }
        cur += c;
        inToken = true;
    }
    if (inToken) out.push_back(cur);
    return out;
}

} // namespace gdip
