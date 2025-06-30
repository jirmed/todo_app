#include "RestServer.h"

#include "service/TaskManager.h"
#include <crow.h>
#include <nlohmann/json.hpp>
#include "presentation/http/TaskMapper.h"
#include "presentation/http/dto/TaskDto.h"
#include "presentation/http/dto/CreateTaskDto.h"
#include "presentation/http/dto/UpdateTaskDto.h"
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

    const std::string MSG_TASK_REMOVED = "Task removed successfully";
    const std::string MSG_TASK_UPDATED = "Task updated";
    const std::string MSG_TASK_NOT_FOUND = "Task not found";
    const std::string MSG_INVALID_JSON = "Invalid JSON format or missing required fields";
    const std::string MSG_INVALID_REQUEST = "Invalid request";
    const std::string MSG_INTERNAL_ERROR = "Internal server error";
}

RestServer::RestServer(TaskManager &manager) : manager_(manager) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
#endif
}

void RestServer::run() const {
    crow::SimpleApp app;
    setupRoutes(app);
    app.port(DEFAULT_PORT).run();
}

void RestServer::setupRoutes(crow::SimpleApp &app) const {
    CROW_ROUTE(app, "/api/tasks").methods(crow::HTTPMethod::GET)([this]() {
        return handleGetAllTasks();
    });
    CROW_ROUTE(app, "/api/tasks").methods(crow::HTTPMethod::POST)([this](const crow::request &req) {
        return handleAddTask(req);
    });
    CROW_ROUTE(app, "/api/tasks/<int>").methods(crow::HTTPMethod::Delete)([this](int id) {
        return handleRemoveTask(id);
    });
    CROW_ROUTE(app, "/api/tasks/<int>").methods(crow::HTTPMethod::PATCH)
    ([this](const crow::request &req, int id) {
        return handleUpdateTask(req, id);
    });
}

crow::response RestServer::handleGetAllTasks() const {
    const auto tasks = manager_.getAllTasks();
    const auto jsonResponse = convertTasksToJson(tasks);
    return createJsonResponse(HTTP_OK, jsonResponse.dump());
}

crow::response RestServer::handleAddTask(const crow::request &req) const {
    try {
        const auto json = nlohmann::json::parse(req.body);
        const auto dto = json.get<CreateTaskDto>();
        // ↓↓↓ ZDE JE ÚPRAVA PRO REST CHOVÁNÍ ↓↓↓
        Task task = manager_.addTask(dto.title); // Vrací nově vytvořený Task
        auto dtoResponse = TaskMapper::toDto(task);
        auto jsonResponse = nlohmann::json(dtoResponse);
        return createJsonResponse(HTTP_CREATED, jsonResponse.dump());
    } catch ([[maybe_unused]] const nlohmann::json::exception &e) {
        return createTextResponse(HTTP_BAD_REQUEST, MSG_INVALID_JSON);
    } catch ([[maybe_unused]] const std::exception &e) {
        return createTextResponse(HTTP_INTERNAL_ERROR, MSG_INTERNAL_ERROR);
    }
}

nlohmann::json RestServer::convertTasksToJson(const std::vector<Task> &tasks) {
    nlohmann::json jsonResponse = nlohmann::json::array();
    for (const auto &task: tasks) {
        const auto dto = TaskMapper::toDto(task);
        jsonResponse.push_back(dto);
    }
    return jsonResponse;
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
    } catch ([[maybe_unused]] const nlohmann::json::exception &e) {
        return createTextResponse(HTTP_BAD_REQUEST, MSG_INVALID_JSON);
    } catch ([[maybe_unused]] const std::exception &e) {
        return createTextResponse(HTTP_INTERNAL_ERROR, MSG_INTERNAL_ERROR);
    }
}

crow::response RestServer::processTaskUpdate(const UpdateTaskDto &dto, int id) const {
    if (!dto.done) {
        return createTextResponse(HTTP_BAD_REQUEST, MSG_INVALID_REQUEST);
    }

    const bool success = manager_.markDoneById(id);
    if (!success) {
        return createTextResponse(HTTP_NOT_FOUND, MSG_TASK_NOT_FOUND);
    }

    // Zde můžeš vracet buď aktualizovaný task jako JSON, nebo jen text.
    // Pro REST správnost doporučuji JSON, např.:
    // Task updatedTask = manager_.getTaskById(id); (pokud máš)
    // auto dtoResponse = TaskMapper::toDto(updatedTask);
    // auto jsonResponse = nlohmann::json(dtoResponse);
    // return createJsonResponse(HTTP_OK, jsonResponse.dump());
    return createTextResponse(HTTP_OK, MSG_TASK_UPDATED);
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
