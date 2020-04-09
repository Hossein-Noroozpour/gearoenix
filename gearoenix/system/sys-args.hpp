#ifndef GEAROENIX_SYSTEM_ARGS_HPP
#define GEAROENIX_SYSTEM_ARGS_HPP
#include "../core/cr-build-configuration.hpp"
#include "../core/cr-static.hpp"
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace gearoenix::system {
class Args {
public:
    typedef std::multimap<std::string, std::optional<std::string>> Map;
    GX_GET_CREF_PRV(std::string, process_name)
    GX_GET_CREF_PRV(std::vector<std::string>, tokens)
    GX_GET_CREF_PRV(Map, map)
    GX_GET_CVAL_PRV(bool, has_tokens)
public:
    Args(const int argc, const char* const* const argv) noexcept
        : process_name(argv[0])
        , tokens(argc - 1)
        , has_tokens(tokens.size() != 0)
    {
        for (int ai = 1, ti = 0; ai < argc; ++ai, ++ti)
            tokens[ti] = std::string(argv[ai]);

        for (std::size_t ti = 0; ti < tokens.size();) {
            const auto& token = tokens[ti];
            if (!is_key(token)) {
                GXLOGF("Unexpected token '" << token << "'.")
            }
            const auto key = token.substr(2);
            ++ti;
            if (ti >= tokens.size()) {
                map.insert(std::make_pair(key, std::nullopt));
                break;
            }
            const auto& next_token = tokens[ti];
            if (is_key(next_token)) {
                map.insert(std::make_pair(key, std::nullopt));
                continue;
            }
            map.insert(std::make_pair(key, next_token));
            ++ti;
        }
    }

    [[nodiscard]] static bool is_key(const std::string& s) noexcept
    {
        return s[0] == '-' && s[1] == '-';
    }

    void get_value(const std::string& key, std::string& value) const noexcept
    {
        auto range = map.equal_range(key);
        int count = 0;
        for (auto i = range.first; i != range.second; ++i, ++count)
            if (i->second.has_value())
                value = i->second.value();
            else
                GXLOGF("The '" << key << "' key must have value.");
        if (count < 1) {
            GXLOGF("The '" << key << "' key is needed and must have value.");
        } else if (count > 1) {
            GXLOGF("The '" << key << "' key is needed only once.");
        }
    }

    void get_value(const std::string& key, int& value) const noexcept
    {
        std::string v;
        get_value(key, v);
        value = std::stoi(v);
    }
};
}

#endif