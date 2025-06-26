#include "service/TaskManager.h"
#include "http/RestServer.h"
#include "service/InMemoryTaskRepository.h"
#include "service/FileRepository.h"

int main() {
//    auto repository = std::make_unique<InMemoryTaskRepository>();
    auto repository = std::make_unique<FileRepository>("data/tasks.csv");
    TaskManager manager(std::move(repository));
    // std::unique_ptr<UI> ui = std::make_unique<ConsoleUI>(manager);
    // ui->run();

    RestServer server(manager);
    server.run();

    return 0;
}