#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace todo::api {

    struct VersionDto {
        std::string version;     // e.g. "1.0.6-SNAPSHOT" or "1.0.5"
        std::string git_tag;     // e.g. "v1.0.5"
        std::string git_sha;     // e.g. "abc1234"
        std::string build_time;  // ISO-like string
        std::string build_type;  // Debug/Release
        std::string compiler;    // e.g. "GCC 13.2.0"
        std::string platform;    // e.g. "Linux/arm64"
    };

    inline void to_json(nlohmann::json& j, const VersionDto& v) {
        j = nlohmann::json{
            {"version",    v.version},
            {"gitTag",     v.git_tag},
            {"gitSha",     v.git_sha},
            {"buildTime",  v.build_time},
            {"buildType",  v.build_type},
            {"compiler",   v.compiler},
            {"platform",   v.platform}
        };
    }

} // namespace todo::api
