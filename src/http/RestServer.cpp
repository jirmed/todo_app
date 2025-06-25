#include "RestServer.h"
#include <crow.h>
#include <nlohmann/json.hpp>
#include "TaskMapper.h"
#include "TaskDto.h"
#include "CreateTaskDto.h"
#include "UpdateTaskDto.h"
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

    setupRoutes(app);

    app.port(18080).run();
}

void RestServer::setupRoutes(crow::SimpleApp &app) {
    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::GET)([this]() {
        return handleGetAllTasks();
    });

    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::POST)([this](const crow::request &req) {
        return handleAddTask(req);
    });

    CROW_ROUTE(app, "/tasks/<int>").methods(crow::HTTPMethod::Delete)([this](int id) {
        return handleRemoveTask(id);
    });

    CROW_ROUTE(app, "/tasks/<int>").methods(crow::HTTPMethod::PATCH)
    ([this](const crow::request &req, int id) {
        return handleUpdateTask(req, id);
    });
}

crow::response RestServer::handleGetAllTasks() {
    const auto tasks = manager_.getAllTasks();
    const auto jsonResponse = convertTasksToJson(tasks);
    return createJsonResponse(200, jsonResponse.dump());
}

crow::response RestServer::handleAddTask(const crow::request &req) {
    try {
        const auto json = nlohmann::json::parse(req.body);
        const CreateTaskDto dto = json.get<CreateTaskDto>();

        manager_.addTask(dto.title);
        return createTextResponse(201, "Task created successfully");
    } catch (const nlohmann::json::exception &e) {
        return createTextResponse(400, "Invalid JSON format or missing required fields");
    } catch (const std::exception &e) {
        return createTextResponse(500, "Internal server error");
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

crow::response RestServer::handleRemoveTask(int id) {
    if (manager_.removeTaskById(id)) {
        return createTextResponse(200, "Task removed successfully");
    } else {
        return createTextResponse(404, "Task not found");
    }
}

crow::response RestServer::handleUpdateTask(const crow::request &req, int id) {
    try {
        const auto json = nlohmann::json::parse(req.body);
        const UpdateTaskDto dto = json.get<UpdateTaskDto>();

        if (dto.done) {
            const bool success = manager_.markDoneById(id);

            if (!success) {
                return createTextResponse(404, "Task not found");
            }

            return createTextResponse(200, "Task updated");
        } else return createTextResponse(400, "Invalid request");
    } catch (const nlohmann::json::exception &e) {
        return createTextResponse(400, "Invalid JSON format");
    } catch (const std::exception &e) {
        return createTextResponse(500, "Internal server error");
    }
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
