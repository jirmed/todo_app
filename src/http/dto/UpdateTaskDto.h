#pragma once

struct UpdateTaskDto {
    bool done;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UpdateTaskDto, done)
};
