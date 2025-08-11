#include <nlohmann/json.hpp>
#include "todo/build/version_dto.hpp"
#include "todo/build/build_info.hpp"

namespace todo::api {

    [[nodiscard]] VersionDto current_version() {
        VersionDto v{};
        v.version    = std::string(buildinfo::APP_VERSION);
        v.git_tag    = std::string(buildinfo::GIT_TAG);
        v.git_sha    = std::string(buildinfo::GIT_SHA);
        v.build_time = std::string(buildinfo::BUILD_TIME);
        v.build_type = std::string(buildinfo::BUILD_TYPE);
        v.compiler   = std::string(buildinfo::COMPILER);
        v.platform   = std::string(buildinfo::PLATFORM);
        return v;
    }

} // namespace todo::api
