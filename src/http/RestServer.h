// RestServer.h
#pragma once
#include "TaskManager.h"
#include <crow.h>
#include <nlohmann/json.hpp>
#include <vector>

#include "RestServer.h"

class RestServer {
private:
    static constexpr const char *JSON_CONTENT_TYPE = "application/json; charset=utf-8";
    static constexpr const char *TEXT_CONTENT_TYPE = "text/plain; charset=utf-8";

    TaskManager &manager_;

    // Helper metody pro zpracování požadavků
    crow::response handleGetAllTasks();

    crow::response handleAddTask(const crow::request &req);

    // Helper metody pro vytváření odpovědí
    crow::response createJsonResponse(int statusCode, const std::string &content);

    crow::response createTextResponse(int statusCode, const std::string &content);

    // Konverzní metody
    nlohmann::json convertTasksToJson(const std::vector<Task> &tasks);

public:
    explicit RestServer(TaskManager &manager);

    void run();

    void setupRoutes(crow::SimpleApp &app);
};
