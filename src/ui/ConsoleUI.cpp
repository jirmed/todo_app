#include "ConsoleUI.h"
#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

namespace {
    // Menu opcí
    constexpr int MENU_SHOW_TASKS = 1;
    constexpr int MENU_ADD_TASK = 2;
    constexpr int MENU_REMOVE_TASK = 3;
    constexpr int MENU_MARK_DONE = 4;
    constexpr int MENU_EXIT = 5;

    // Textové zprávy
    constexpr auto MSG_TASK_ADDED = "Úkol byl přidán!";
    constexpr auto MSG_TASK_REMOVED = "Úkol byl odstraněn!";
    constexpr auto MSG_TASK_COMPLETED = "Úkol byl označen jako dokončený!";
    constexpr auto MSG_INVALID_TASK_NUMBER = "Neplatné číslo úkolu!";
}

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
        case MENU_SHOW_TASKS:
            displayTasks(manager_.tasks());
            break;
        case MENU_ADD_TASK:
            handleAddTask();
            break;
        case MENU_REMOVE_TASK:
            handleRemoveTask();
            break;
        case MENU_MARK_DONE:
            handleMarkDone();
            break;
        case MENU_EXIT:
            exitApplication();
            break;
        default:
            showInvalidChoiceMessage();
            break;
    }
}

void ConsoleUI::handleAddTask() {
    manager_.addTask(promptForNewTaskTitle());
    notifySuccess(MSG_TASK_ADDED);
}

void ConsoleUI::handleRemoveTask() {
    if (manager_.removeTask(promptForTaskIndex())) {
        notifySuccess(MSG_TASK_REMOVED);
    } else {
        notifyError(MSG_INVALID_TASK_NUMBER);
    }
}

void ConsoleUI::handleMarkDone() {
    if (manager_.markDone(promptForTaskIndex())) {
        notifySuccess(MSG_TASK_COMPLETED);
    } else {
        notifyError(MSG_INVALID_TASK_NUMBER);
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
