#include <service/TaskManager.h>
#include <presentation/http/RestServer.h>

#include "repository/file/FileRepository.h"
#include "repository/memory/InMemoryTaskRepository.h"

// Přidáme potřebné hlavičkové soubory i sem
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

int main() {
    // Nastavení konzole pro UTF-8 (pouze pro Windows)
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
#endif

    // auto repository = std::make_unique<InMemoryTaskRepository>();
    auto repository = std::make_unique<FileRepository>("tasks.csv");
    TaskManager manager(std::move(repository));
    // std::unique_ptr<UI> ui = std::make_unique<ConsoleUI>(manager);
    // ui->run();

    RestServer server(manager);
    server.run();

    return 0;
}