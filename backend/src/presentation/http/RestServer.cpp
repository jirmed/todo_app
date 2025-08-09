#include "RestServer.h"
#include "service/TaskManager.h"
#include "presentation/http/TaskMapper.h"
#include "presentation/http/dto/CreateTaskDto.h"
#include "presentation/http/dto/UpdateTaskDto.h"
#include "presentation/http/dto/TaskDto.h"
#include <nlohmann/json.hpp>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

namespace {
    constexpr int DEFAULT_PORT = 18080;
    constexpr int HTTP_OK = 200;
    constexpr int HTTP_CREATED = 201;
    constexpr int HTTP_BAD_REQUEST = 400;
    constexpr int HTTP_NOT_FOUND = 404;
    constexpr int HTTP_INTERNAL_ERROR = 500;
    const std::string JSON_CONTENT_TYPE = "application/json; charset=utf-8";
    const std::string TEXT_CONTENT_TYPE = "text/plain; charset=utf-8";
    const std::string MSG_TASK_REMOVED = "Task removed successfully";
    const std::string MSG_TASK_UPDATED = "Task updated";
    const std::string MSG_TASK_NOT_FOUND = "Task not found";
    const std::string MSG_INVALID_JSON = "Invalid JSON format or missing required fields";
    const std::string MSG_INVALID_REQUEST = "Invalid request";
    const std::string MSG_INTERNAL_ERROR = "Internal server error";
}

RestServer::RestServer(TaskManager &manager) : manager_(manager) {
    setupRoutes();
}

void RestServer::run() {
    app_.port(DEFAULT_PORT).run();
}

void RestServer::setupRoutes() {
    // Health & readiness
    CROW_ROUTE(app_, "/health").methods(crow::HTTPMethod::GET)([this]() {
        return handleHealth();
    });
    CROW_ROUTE(app_, "/ready").methods(crow::HTTPMethod::GET)([this]() {
        return handleReady();
    });

    // Tasks API
    CROW_ROUTE(app_, "/api/tasks").methods(crow::HTTPMethod::GET)([this]() {
        return handleGetAllTasks();
    });
    CROW_ROUTE(app_, "/api/tasks").methods(crow::HTTPMethod::POST)([this](const crow::request &req) {
        return handleAddTask(req);
    });
    CROW_ROUTE(app_, "/api/tasks/<int>").methods(crow::HTTPMethod::Delete)([this](int id) {
        return handleRemoveTask(id);
    });
    CROW_ROUTE(app_, "/api/tasks/<int>").methods(crow::HTTPMethod::PATCH)
    ([this](const crow::request &req, int id) {
        return handleUpdateTask(req, id);
    });
}

crow::response RestServer::handleHealth() const {
    // jednoduchá odpověď 200 OK
    nlohmann::json j;
    j["status"] = "ok";
    return createJsonResponse(HTTP_OK, j.dump());
}

crow::response RestServer::handleReady() const {
    // Ověř, že umíme sáhnout na „základní závislosti“ (tady TaskManager)
    try {
        (void)manager_.getAllTasks();
        nlohmann::json j;
        j["status"] = "ready";
        return createJsonResponse(HTTP_OK, j.dump());
    } catch (const std::exception &) {
        nlohmann::json j;
        j["status"] = "unavailable";
        return createJsonResponse(HTTP_INTERNAL_ERROR, j.dump());
    }
}

crow::response RestServer::handleGetAllTasks() const {
    const auto tasks = manager_.getAllTasks();
    const auto jsonResponse = convertTasksToJson(tasks);
    return createJsonResponse(HTTP_OK, jsonResponse.dump());
}

crow::response RestServer::handleAddTask(const crow::request &req) const {
    try {
        const auto json = nlohmann::json::parse(req.body);
        const auto [title] = json.get<CreateTaskDto>();
        const Task task = manager_.addTask(title);
        auto dtoResponse = TaskMapper::toDto(task);
        const auto jsonResponse = nlohmann::json(dtoResponse);
        return createJsonResponse(HTTP_CREATED, jsonResponse.dump());
    } catch (const nlohmann::json::exception &) {
        return createTextResponse(HTTP_BAD_REQUEST, MSG_INVALID_JSON);
    } catch (const std::exception &) {
        return createTextResponse(HTTP_INTERNAL_ERROR, MSG_INTERNAL_ERROR);
    }
}

crow::response RestServer::handleRemoveTask(int id) const {
    if (manager_.removeTaskById(id)) {
        return createTextResponse(HTTP_OK, MSG_TASK_REMOVED);
    } else {
        return createTextResponse(HTTP_NOT_FOUND, MSG_TASK_NOT_FOUND);
    }
}

crow::response RestServer::handleUpdateTask(const crow::request &req, int id) const {
    try {
        const auto json = nlohmann::json::parse(req.body);
        const UpdateTaskDto dto = json.get<UpdateTaskDto>();
        return processTaskUpdate(dto, id);
    } catch (const nlohmann::json::exception &) {
        return createTextResponse(HTTP_BAD_REQUEST, MSG_INVALID_JSON);
    } catch (const std::exception &) {
        return createTextResponse(HTTP_INTERNAL_ERROR, MSG_INTERNAL_ERROR);
    }
}

crow::response RestServer::processTaskUpdate(const UpdateTaskDto &dto, int id) const {
    if (!dto.done) {
        return createTextResponse(HTTP_BAD_REQUEST, MSG_INVALID_REQUEST);
    }
    if (!manager_.markDoneById(id)) {
        return createTextResponse(HTTP_NOT_FOUND, MSG_TASK_NOT_FOUND);
    }
    return createTextResponse(HTTP_OK, MSG_TASK_UPDATED);
}

nlohmann::json RestServer::convertTasksToJson(const std::vector<Task> &tasks) {
    nlohmann::json jsonResponse = nlohmann::json::array();
    for (const auto &task: tasks) {
        const auto dto = TaskMapper::toDto(task);
        jsonResponse.push_back(dto);
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
