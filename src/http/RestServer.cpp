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

void RestServer::run() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/getAllTasks").methods(crow::HTTPMethod::GET)([this]() {
        return handleGetAllTasks();
    });

    CROW_ROUTE(app, "/addTask").methods(crow::HTTPMethod::POST)([this](const crow::request &req) {
        return handleAddTask(req);
    });

    app.port(18080).run();
}

crow::response RestServer::handleGetAllTasks() {
    const auto tasks = manager_.getAllTasks();
    const auto jsonResponse = convertTasksToJson(tasks);
    return createJsonResponse(200, jsonResponse.dump());
}

crow::response RestServer::handleAddTask(const crow::request &req) {
    try {
        const auto json = nlohmann::json::parse(req.body);

        if (!json.contains("title")) {
            return createTextResponse(400, "Missing 'title' field");
        }

        const std::string title = json["title"];
        manager_.addTask(title);

        return createTextResponse(201, "Task created successfully");
    } catch (const std::exception &e) {
        return createTextResponse(400, "Invalid JSON format");
    }
}

nlohmann::json RestServer::convertTasksToJson(const std::vector<Task> &tasks) {
    nlohmann::json jsonResponse = nlohmann::json::array();

    for (const auto &task: tasks) {
        const auto dto = TaskMapper::toDto(task);
        nlohmann::json taskJson;
        taskJson["title"] = dto.title;
        taskJson["done"] = dto.completed;
        jsonResponse.push_back(taskJson);
    }

    return jsonResponse;
}

crow::response RestServer::createJsonResponse(int statusCode, const std::string &content) {
    crow::response res(statusCode, content);
    res.add_header("Content-Type", JSON_CONTENT_TYPE);
    return res;
}

crow::response RestServer::createTextResponse(int statusCode, const std::string &content) {
    crow::response res(statusCode, content);
    res.add_header("Content-Type", TEXT_CONTENT_TYPE);
    return res;
}