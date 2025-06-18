#include "ConsoleUI.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif


ConsoleUI::ConsoleUI(TaskManager &manager) : manager_(manager) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void ConsoleUI::run() {
    std::cout << "=== TODO APLIKACE ===\n";

    while (true) {
        showMenu();
        handleUserChoice();
    }
}

void ConsoleUI::showMenu() {
    std::cout << "\n--- MENU ---\n";
    std::cout << "1. Zobrazit úkoly\n";
    std::cout << "2. Přidat úkol\n";
    std::cout << "3. Odstranit úkol\n";
    std::cout << "4. Ukončit\n";
    std::cout << "Vyberte možnost (1-4): ";
}

void ConsoleUI::handleUserChoice() {
    int choice = getUserChoice();

    switch (choice) {
        case 1:
            manager_.printTasks();
            break;

        case 2: {
            std::cout << "Zadejte název úkolu: ";
            std::string title;
            std::getline(std::cin >> std::ws, title);
            manager_.addTask(title);
            std::cout << "Úkol byl přidán!\n";
            break;
        }

        case 3: {
            manager_.printTasks();
            std::cout << "Zadejte číslo úkolu k odstranění (začínáme od 1): ";
            size_t index;
            std::cin >> index;
            std::cin.ignore();

            if (index > 0 && manager_.removeTask(index - 1)) {
                std::cout << "Úkol byl odstraněn!\n";
            } else {
                std::cout << "Neplatné číslo úkolu!\n";
            }
            break;
        }

        case 4:
            std::cout << "Ukončuji aplikaci...\n";
            std::exit(0);

        default:
            std::cout << "Neplatná volba! Zkuste to znovu.\n";
            break;
    }
}

int ConsoleUI::getUserChoice() {
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    return choice;
}
