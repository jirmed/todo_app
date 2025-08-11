#pragma once

#include "service/TaskManager.h"
#include <crow.h>
#include <string>

// DTO struktura pro update, pokud je potřeba v hlavičce
struct UpdateTaskDto;

class RestServer {
public:
    // Konstruktor, který přijímá referenci na TaskManager
    explicit RestServer(TaskManager &manager);

    // Metoda pro spuštění serveru v produkci
    void run();

    // --- VEŘEJNÉ METODY PRO PŘÍMÉ VOLÁNÍ A TESTOVÁNÍ ---
    [[nodiscard]] crow::response handleGetAllTasks() const;

    [[nodiscard]] crow::response handleAddTask(const crow::request &req) const;

    [[nodiscard]] crow::response handleRemoveTask(int id) const;

    [[nodiscard]] crow::response handleUpdateTask(const crow::request &req, int id) const;

    // Zdravotní endpoint (interní pro healthcheck)
    [[nodiscard]] crow::response handleHealth() const;

private:
    void setupRoutes();

    [[nodiscard]] crow::response processTaskUpdate(const UpdateTaskDto &dto, int id) const;

    static nlohmann::json convertTasksToJson(const std::vector<Task> &tasks);

    static crow::response createJsonResponse(int statusCode, const std::string &content);

    static crow::response createTextResponse(int statusCode, const std::string &content);

    // Členské proměnné
    TaskManager &manager_;
    crow::SimpleApp app_; // Crow aplikace je nyní členskou proměnnou
};
