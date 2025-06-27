// RestServer.h
#pragma once
#include "service/TaskManager.h"
#include <crow.h>
#include <nlohmann/json.hpp>
#include <vector>

#include "RestServer.h"
#include "dto/UpdateTaskDto.h"

class RestServer {
    // Friend deklarace na začátku třídy
    friend class RestServerTest;
    
private:
    static constexpr const char *JSON_CONTENT_TYPE = "application/json; charset=utf-8";
    static constexpr const char *TEXT_CONTENT_TYPE = "text/plain; charset=utf-8";

    TaskManager &manager_;

    // Helper metody pro zpracování požadavků
    crow::response handleGetAllTasks() const;
    crow::response handleAddTask(const crow::request &req) const;
    crow::response handleRemoveTask(int id) const;
    crow::response handleUpdateTask(const crow::request &req, int id) const;
    crow::response processTaskUpdate(const UpdateTaskDto &dto, int id) const;

    // Statické helper metody pro vytváření odpovědí
    static crow::response createJsonResponse(int statusCode, const std::string &content);
    static crow::response createTextResponse(int statusCode, const std::string &content);

    // Statická konverzní metoda
    static nlohmann::json convertTasksToJson(const std::vector<Task> &tasks);

public:
    explicit RestServer(TaskManager &manager);

    void run() const;

    void setupRoutes(crow::SimpleApp &app) const;

    // Veřejné testovací metody pro přístup k privátním handleru metodám
    crow::response testHandleGetAllTasks() const { return handleGetAllTasks(); }
    crow::response testHandleAddTask(const crow::request &req) const { return handleAddTask(req); }
    crow::response testHandleRemoveTask(int id) const { return handleRemoveTask(id); }
};