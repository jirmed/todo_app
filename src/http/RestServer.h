// RestServer.h
#pragma once
#include "TaskManager.h"
#include <crow.h>
#include <nlohmann/json.hpp>
#include <vector>

#include "RestServer.h"
#include "UpdateTaskDto.h"

class RestServer {
    // Friend deklarace na začátku třídy
    friend class RestServerTest;
    
private:
    static constexpr const char *JSON_CONTENT_TYPE = "application/json; charset=utf-8";
    static constexpr const char *TEXT_CONTENT_TYPE = "text/plain; charset=utf-8";

    TaskManager &manager_;

    // Helper metody pro zpracování požadavků
    crow::response handleGetAllTasks();
    crow::response handleAddTask(const crow::request &req);
    crow::response handleRemoveTask(int id);
    crow::response handleUpdateTask(const crow::request &req, int id);
    crow::response processTaskUpdate(const UpdateTaskDto &dto, int id);

    // Statické helper metody pro vytváření odpovědí
    static crow::response createJsonResponse(int statusCode, const std::string &content);
    static crow::response createTextResponse(int statusCode, const std::string &content);

    // Statická konverzní metoda
    static nlohmann::json convertTasksToJson(const std::vector<Task> &tasks);

public:
    explicit RestServer(TaskManager &manager);

    void run();

    void setupRoutes(crow::SimpleApp &app);

    // Veřejné testovací metody pro přístup k privátním handleru metodám
    crow::response testHandleGetAllTasks() { return handleGetAllTasks(); }
    crow::response testHandleAddTask(const crow::request &req) { return handleAddTask(req); }
    crow::response testHandleRemoveTask(int id) { return handleRemoveTask(id); }
};