#include "Messages.h"

#include <unordered_map>

namespace messages {

    namespace {
        const std::unordered_map<MessageKey, std::string_view> MESSAGE_MAP = {
#define X(key, text) { MessageKey::key, text },
#include "Messages.def"

#undef X
        };
    }

    std::string_view get(MessageKey key) {
        return MESSAGE_MAP.at(key);
    }

} // namespace messages
