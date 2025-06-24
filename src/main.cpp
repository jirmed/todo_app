#include <iostream>
#include <string>
#include <memory>
#include "TaskManager.h"
#include "http/RestServer.h"
#include "ui/ConsoleUI.h"
#include "ui/UI.h"

int main() {
    TaskManager manager;
    // std::unique_ptr<UI> ui = std::make_unique<ConsoleUI>(manager);
    // ui->run();

    RestServer server(manager);
    server.run();

    return 0;
}