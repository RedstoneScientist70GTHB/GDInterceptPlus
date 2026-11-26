#pragma once

// Importing a pasted curl command.

#include <algorithm>
#include <string>
#include <vector>

#include "shell.hpp"
#include "text.hpp"
#include "types.hpp"

namespace gdip {

// Turn a pasted curl command (e.g. from GDIntercept's "Copy Curl") into a draft.
inline bool parseCurl(std::string const& text, Draft& out) {
    auto tokens = shellSplit(text);
    if (tokens.empty()) return false;

    Draft d;
    size_t i = 0;
    if (tokens[0] == "curl" || tokens[0] == "curl.exe") i = 1;

    bool explicitMethod = false;
    std::vector<std::string> bodies;
    auto next = [&]() -> std::string { return (i + 1 < tokens.size()) ? tokens[++i] : std::string(); };

    for (; i < tokens.size(); ++i) {
        std::string const& a = tokens[i];
        if (a == "-X" || a == "--request") {
            d.method = toUpper(next());
            explicitMethod = true;
        } else if (a == "-H" || a == "--header") {
            std::string h = next();
            if (!d.headers.empty()) d.headers += "\\n";
            d.headers += h;
        } else if (a == "-A" || a == "--user-agent") {
            std::string ua = next();
            if (!d.headers.empty()) d.headers += "\\n";
            d.headers += "User-Agent: " + ua;
        } else if (a == "-d" || a == "--data" || a == "--data-raw" || a == "--data-binary" ||
                   a == "--data-ascii" || a == "--data-urlencode") {
            bodies.push_back(next());
        } else if (a == "--url") {
            d.url = next();
        } else if (a.rfind("http://", 0) == 0 || a.rfind("https://", 0) == 0) {
            d.url = a;
        }
        // Everything else (-s, -k, --compressed, ...) is ignored.
    }

    for (size_t k = 0; k < bodies.size(); ++k) {
        if (k) d.body += "&";
        d.body += bodies[k];
    }
    if (!bodies.empty() && !explicitMethod) d.method = "POST";

    if (d.url.empty()) return false;
    if (!d.method.empty() && std::find(methods().begin(), methods().end(), d.method) == methods().end()) {
        d.method = "GET";
    }
    out = std::move(d);
    return true;
}

} // namespace gdip
