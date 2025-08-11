// FILE: src/presentation/http/dto/VersionDto.h
#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct VersionDto {
    std::string version;    // e.g., "1.0.6-SNAPSHOT" or "1.0.5"
    std::string gitTag;     // e.g., "v1.0.5"
    std::string gitSha;     // e.g., "abc1234"
    std::string buildTime;  // ISO 8601 UTC
    std::string buildType;  // Debug/Release
    std::string compiler;   // e.g., "GCC 13.2.0"
    std::string platform;   // e.g., "Linux/arm64"
};

inline void to_json(nlohmann::json& j, const VersionDto& v) {
    j = nlohmann::json{
            {"version",   v.version},
            {"gitTag",    v.gitTag},
            {"gitSha",    v.gitSha},
            {"buildTime", v.buildTime},
            {"buildType", v.buildType},
            {"compiler",  v.compiler},
            {"platform",  v.platform}
    };
}
