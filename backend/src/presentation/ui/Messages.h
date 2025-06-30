#pragma once
#include <string_view>

namespace messages {

    enum class MessageKey {
#define X(key, text) key,
#include "Messages.def"

#undef X
    };

    std::string_view get(MessageKey key);

} // namespace messages
