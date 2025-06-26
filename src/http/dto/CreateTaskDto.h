#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct CreateTaskDto {
    std::string title;
    
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CreateTaskDto, title)
};