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

namespace {
    using namespace messages;
    using std::cout;
    using std::cin;
    using std::endl;
}

void ConsoleUI::run() {
    cout << APP_TITLE << endl;
    while (true) {
        showMenu();
        handleUserChoice();
    }
}

void ConsoleUI::showMenu() {
    cout << MENU_TITLE << endl;
    cout << MENU_SHOW_TASKS << endl;
    cout << MENU_ADD_TASK << endl;
    cout << MENU_REMOVE_TASK << endl;
    cout << MENU_MARK_DONE << endl;
    cout << MENU_EXIT << endl;
    cout << MENU_PROMPT;
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
    notifySuccess(TASK_ADDED);
}

void ConsoleUI::handleRemoveTask() {
    if (manager_.removeTask(promptForTaskIndex())) {
        notifySuccess(TASK_REMOVED);
    } else {
        notifyError(INVALID_TASK_NUMBER);
    }
}

void ConsoleUI::handleMarkDone() {
    if (manager_.markDone(promptForTaskIndex())) {
        notifySuccess(TASK_COMPLETED);
    } else {
        notifyError(INVALID_TASK_NUMBER);
    }
}

int ConsoleUI::getUserChoice() {
    int choice;
    if (!(cin >> choice)) {
        cin.clear(); // reset error flags
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // vyčistit buffer
        return -1; // nebo jiná hodnota mimo rozsah
    }
    cin.ignore();
    return choice;
}

void ConsoleUI::exitApplication() {
    cout << EXITING << endl;
    std::exit(0);
}

void ConsoleUI::showInvalidChoiceMessage() {
    cout << INVALID_CHOICE << endl;
}

void ConsoleUI::notifySuccess(std::string_view message) {
    cout << "[OK] " << message << endl;
}

void ConsoleUI::notifyInfo(std::string_view message) {
    cout << "[INFO] " << message << endl;
}

void ConsoleUI::notifyError(std::string_view message) {
    cout << "[CHYBA] " << message << endl;
}

void ConsoleUI::displayTasks(const std::vector<Task> &tasks) {
    if (tasks.empty()) {
        cout << NO_TASKS << endl;
        return;
    }

    cout << TASKS_LIST_TITLE << endl;
    for (std::size_t i = 0; i < tasks.size(); ++i) {
        cout << formatTaskItem(i, tasks[i]) << endl;
    }
}

std::string ConsoleUI::formatTaskItem(std::size_t index, const Task &task) {
    const char statusChar = task.done_ ? 'x' : ' ';
    return std::to_string(index) + ". [" + statusChar + "] " + task.title_;
}

std::string ConsoleUI::promptForNewTaskTitle() {
    cout << TASK_TITLE_PROMPT;
    std::string title;
    std::getline(cin >> std::ws, title);
    return title;
}

std::size_t ConsoleUI::promptForTaskIndex() {
    cout << TASK_INDEX_PROMPT;
    std::size_t index;
    cin >> index;
    cin.ignore();
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
