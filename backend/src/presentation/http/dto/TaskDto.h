#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct TaskDto {
    int id;
    std::string title;
    bool done;

    // Automaticky vytvoří to_json a from_json funkce
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TaskDto, id, title, done)
};