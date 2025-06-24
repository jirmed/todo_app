// RestServer.cpp
#include "RestServer.h"
#include <crow.h>  // musí fungovat po opravě include path

RestServer::RestServer(TaskManager& manager) : manager_(manager) {}

void RestServer::run() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/tasks")
    ([this] {
        // např. vrátí JSON pole úkolů
        return crow::json::wvalue{
            {"message", "Zde budou úkoly"}
        };
    });

    app.port(18080).run();
}
