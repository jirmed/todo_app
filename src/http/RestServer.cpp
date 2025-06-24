// RestServer.cpp
#include "RestServer.h"
#include <crow.h>
#include <nlohmann/json.hpp>
#include "TaskMapper.h"
#include "TaskDto.h"
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

RestServer::RestServer(TaskManager &manager) : manager_(manager) {
#ifdef _WIN32
    // Nastavení UTF-8 pro Windows
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
#endif
}

crow::response RestServer::createJsonResponse(int statusCode, const nlohmann::json &json) const {
    crow::response res(statusCode, json.dump());
    res.add_header("Content-Type", "application/json; charset=utf-8");
    return res;
}

crow::response RestServer::createTextResponse(int statusCode, const std::string &message) const {
    crow::response res(statusCode, message);
    res.add_header("Content-Type", "text/plain; charset=utf-8");
    return res;
}

nlohmann::json RestServer::convertTasksToJson(const std::vector<Task> &tasks) const {
    std::vector<TaskDto> dtos;
    for (const auto &task: tasks) {
        dtos.push_back(TaskMapper::toDto(task));
    }

    nlohmann::json jsonResponse = nlohmann::json::array();
    for (const auto &dto: dtos) {
        nlohmann::json taskJson;
        taskJson["title"] = dto.title;
        taskJson["done"] = dto.completed;
        jsonResponse.push_back(taskJson);
    }
    return jsonResponse;
}

crow::response RestServer::handleGetAllTasks() const {
    auto tasks = manager_.getAllTasks();
    auto jsonResponse = convertTasksToJson(tasks);
    return createJsonResponse(200, jsonResponse);
}

crow::response RestServer::handleAddTask(const crow::request &req) {
    try {
        auto json = nlohmann::json::parse(req.body);

        if (!json.contains("title")) {
            return createTextResponse(400, "Missing 'title' field");
        }

        std::string title = json["title"];
        manager_.addTask(title);

        return createTextResponse(201, "Task create