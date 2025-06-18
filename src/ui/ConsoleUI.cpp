#include "ConsoleUI.h"
#include "Messages.h"
#include <iostream>
#include <string>
#include <limits>
#include <optional>
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
    std::cout << messages::APP_TITLE << "\n";
    while (true) {
        showMenu();
        handleUserChoice();
    }
}

void ConsoleUI::showMenu() {
    std::cout << messages::MENU_TITLE << "\n";
    std::cout << messages::MENU_SHOW_TASKS << "\n";
    std::cout << messages::MENU_ADD_TASK << "\n";
    std::cout << messages::MENU_REMOVE_TASK << "\n";
    std::cout << messages::MENU_MARK_DONE << "\n";
    std::cout << messages::MENU_EXIT << "\n";
    std::cout << messages::MENU_PROMPT;
}

void ConsoleUI::handleUserChoice() {
    int choice = getUserChoice();
    if (auto opt = toMenuOption(choice)) {
        switch (*opt) {
            case MenuOption::SHOW_TASKS:
                displayTasks(manager_.tasks());
                break;
            case MenuOption::ADD_TASK:
                handleAddTask();
                break;
            case MenuOption::REMOVE_TASK:
                handleRemoveTask();
                break;
            case MenuOption::MARK_DONE:
                handleMarkDone();
                break;
            case MenuOption::EXIT:
                exitApplication();
                break;
            default:
                showInvalidChoiceMessage();
                break;
        }
    } else {
        showInvalidChoiceMessage();
    }
}

void ConsoleUI::handleAddTask() {
    manager_.addTask(promptForNewTaskTitle());
    notifySuccess(messages::TASK_ADDED);
}

void ConsoleUI::handleRemoveTask() {
    if (manager_.removeTask(promptForTaskIndex())) {
        notifySuccess(messages::TASK_REMOVED);
    } else {
        notifyError(messages::INVALID_TASK_NUMBER);
    }
}

void ConsoleUI::handleMarkDone() {
    if (manager_.markDone(promptForTaskIndex())) {
        notifySuccess(messages::TASK_COMPLETED);
    } else {
        notifyError(messages::INVALID_TASK_NUMBER);
    }
}

int ConsoleUI::getUserChoice() {
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear(); // reset error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // vyčistit buffer
        return -1; // nebo jiná hodnota mimo rozsah
    }
    std::cin.ignore();
    return choice;
}

void ConsoleUI::exitApplication() {
    std::cout << messages::EXITING << "\n";
    std::exit(0);
}

void ConsoleUI::showInvalidChoiceMessage() {
    std::cout << messages::INVALID_CHOICE << "\n";
}

void ConsoleUI::notifySuccess(std::string_view message) {
    std::cout << "[OK] " << message << "\n";
}

void ConsoleUI::notifyInfo(std::string_view message) {
    std::cout << "[INFO] " << message << "\n";
}

void ConsoleUI::notifyError(std::string_view message) {
    std::cout << "[CHYBA] " << message << "\n";
}

void ConsoleUI::displayTasks(const std::vector<Task> &tasks) {
    if (tasks.empty()) {
        std::cout << messages::NO_TASKS << "\n";
        return;
    }

    std::cout << messages::TASKS_LIST_TITLE << "\n";
    for (std::size_t i = 0; i < tasks.size(); ++i) {
        std::cout << formatTaskItem(i, tasks[i]) << "\n";
    }
}

std::string ConsoleUI::formatTaskItem(std::size_t index, const Task &task) {
    const char statusChar = task.done_ ? 'x' : ' ';
    return std::to_string(index) + ". [" + statusChar + "] " + task.title_;
}

std::string ConsoleUI::promptForNewTaskTitle() {
    std::cout << messages::TASK_TITLE_PROMPT;
    std::string title;
    std::getline(std::cin >> std::ws, title);
    return title;
}

std::size_t ConsoleUI::promptForTaskIndex() {
    std::cout << messages::TASK_INDEX_PROMPT;
    std::size_t index;
    std::cin >> index;
    std::cin.ignore();
    return index;
}

std::optional<MenuOption> ConsoleUI::toMenuOption(int value) {
    switch (value) {
        case 1: return MenuOption::SHOW_TASKS;
        case 2: return MenuOption::ADD_TASK;
        case 3: return MenuOption::REMOVE_TASK;
        case 4: return MenuOption::MARK_DONE;
        case 5: return MenuOption::EXIT;
        default: return std::nullopt;
    }
}
