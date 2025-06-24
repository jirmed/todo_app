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

RestServer::RestServer(TaskManager& manager) : manager_(manager) {
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

    // GET /getAllTasks
    CROW_ROUTE(app, "/getAllTasks").methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
        auto tasks = manager_.getAllTasks();
        std::vector<TaskDto> dtos;
        for (const auto& task : tasks) {
            dtos.push_back(TaskMapper::toDto(task));
        }
        
        nlohmann::json jsonResponse = nlohmann::json::array();
        for (const auto& dto : dtos) {
            nlohmann::json taskJson;
            taskJson["title"] = dto.title;
            taskJson["done"] = dto.completed;
            jsonResponse.push_back(taskJson);
        }
        
        // Přidání správných UTF-8 hlaviček
        crow::response res(200, jsonResponse.dump());
        res.add_header("Content-Type", "application/json; charset=utf-8");
        return res;
    });

    // POST /addTask
    CROW_ROUTE(app, "/addTask").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        try {
            auto json = nlohmann::json::parse(req.body);
            
            if (!json.contains("title")) {
                crow::response res(400, "Missing 'title' field");
                res.add_header("Content-Type", "text/plain; charset=utf-8");
                return res;
            }
            
            std::string title = json["title"];
            manager_.addTask(title);
            
            crow::response res(201, "Task created successfully");
            res.add_header("Content-Type", "text/plain; charset=utf-8");
            return res;
        } catch (const std::exception& e) {
            crow::response res(400, "Invalid JSON format");
            res.add_header("Content-Type", "text/plain; charset=utf-8");
            return res;
        }
    });

    app.port(18080).run();
}