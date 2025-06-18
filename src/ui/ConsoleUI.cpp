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
    std::cout << "4. Označit úkol jako dokončený\n";
    std::cout << "5. Ukončit\n";
    std::cout << "Vyberte možnost (1-5): ";
}

void ConsoleUI::handleUserChoice() {
    switch (int choice = getUserChoice()) {
        case 1:
            displayTasks(manager_.tasks());
            break;
        case 2: {
            manager_.addTask(promptForNewTaskTitle());
            notifySuccess("Úkol byl přidán!");
            break;
        }
        case 3: {
            if (manager_.removeTask(promptForTaskIndex())) {
                notifySuccess("Úkol byl odstraněn!");
            } else {
                notifyError("Neplatné číslo úkolu!");
            }
            break;
        }case 4: {
            if (manager_.markDone(promptForTaskIndex())) {
                notifySuccess("Úkol byl označen jako dokončený!");
            } else {
                notifyError("Neplatné číslo úkolu!");
            }
            break;
        }
        case 5:
            exitApplication();
            break;
        default:
            showInvalidChoiceMessage();
            break;
    }
}

int ConsoleUI::getUserChoice() {
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    return choice;
}

void ConsoleUI::exitApplication() {
    std::cout << "Ukončuji aplikaci...\n";
    std::exit(0);
}

void ConsoleUI::showInvalidChoiceMessage() {
    std::cout << "Neplatná volba! Zkuste to znovu.\n";
}

void ConsoleUI::notifySuccess(const std::string &message) {
    std::cout << "[OK] " << message << "\n";
}

void ConsoleUI::notifyInfo(const std::string &message) {
    std::cout << "[INFO] " << message << "\n";
}

void ConsoleUI::notifyError(const std::string &message) {
    std::cout << "[CHYBA] " << message << "\n";
}

void ConsoleUI::displayTasks(const std::vector<Task> &tasks) {
    if (tasks.empty()) {
        std::cout << "Žádné úkoly.\n";
        return;
    }

    std::cout << "\nSeznam úkolů:\n";
    for (std::size_t i = 0; i < tasks.size(); ++i) {
        const auto &task = tasks[i];
        std::cout << i << ". [" << (task.done_ ? "x" : " ") << "] " << task.title_ << "\n";
    }
}

std::string ConsoleUI::promptForNewTaskTitle() {
    std::cout << "Zadejte název úkolu: ";
    std::string title;
    std::getline(std::cin >> std::ws, title);
    return title;
}

std::size_t ConsoleUI::promptForTaskIndex() {
    std::cout << "Zadejte číslo úkolu: ";
    std::size_t index;
    std::cin >> index;
    std::cin.ignore();
    return index;
}
