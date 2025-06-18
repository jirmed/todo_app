#include "ConsoleUI.h"
#include "Messages.h"
#include <iostream>
#include <string>
#include <limits>
#include <optional>

#ifdef _WIN32
#include <windows.h>
#endif

ConsoleUI::ConsoleUI(TaskManager& manager) : manager_(manager) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

namespace {
    using messages::get;
    using messages::MessageKey;
    using enum MessageKey;  // umožní používat např. APP_TITLE místo MessageKey::APP_TITLE
    using std::cout;
    using std::cin;
    using std::endl;

    // Alias pro get() pro zkrácení zápisu
    auto t = [](MessageKey key) { return get(key); };
}

[[noreturn]] void ConsoleUI::run() {
    cout << t(APP_TITLE) << endl;
    while (true) {
        showMenu();
        handleUserChoice();
    }
}

void ConsoleUI::showMenu() {
    cout << t(MENU_TITLE) << endl;
    cout << t(MENU_SHOW_TASKS) << endl;
    cout << t(MENU_ADD_TASK) << endl;
    cout << t(MENU_REMOVE_TASK) << endl;
    cout << t(MENU_MARK_DONE) << endl;
    cout << t(MENU_EXIT) << endl;
    cout << t(MENU_PROMPT);
}

void ConsoleUI::handleUserChoice() {
    int choice = getUserChoice();
    if (const auto opt = toMenuOption(choice)) {
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
    notifySuccess(t(TASK_ADDED));
}

void ConsoleUI::handleRemoveTask() {
    if (manager_.removeTask(promptForTaskIndex())) {
        notifySuccess(t(TASK_REMOVED));
    } else {
        notifyError(t(INVALID_TASK_NUMBER));
    }
}

void ConsoleUI::handleMarkDone() {
    if (manager_.markDone(promptForTaskIndex())) {
        notifySuccess(t(TASK_COMPLETED));
    } else {
        notifyError(t(INVALID_TASK_NUMBER));
    }
}

int ConsoleUI::getUserChoice() {
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    cin.ignore();
    return choice;
}

void ConsoleUI::exitApplication() {
    cout << t(EXITING) << endl;
    std::exit(0);
}

void ConsoleUI::showInvalidChoiceMessage() {
    cout << t(INVALID_CHOICE) << endl;
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

void ConsoleUI::displayTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        cout << t(NO_TASKS) << endl;
        return;
    }

    cout << t(TASKS_LIST_TITLE) << endl;
    for (std::size_t i = 0; i < tasks.size(); ++i) {
        cout << formatTaskLine(i, tasks[i]) << endl;
    }
}

std::string ConsoleUI::formatTaskLine(std::size_t index, const Task& task) {
    const char statusChar = task.done_ ? 'x' : ' ';
    return std::to_string(index) + ". [" + statusChar + "] " + task.title_;
}

std::string ConsoleUI::promptForNewTaskTitle() {
    cout << t(TASK_TITLE_PROMPT);
    std::string title;
    std::getline(cin >> std::ws, title);
    return title;
}

std::size_t ConsoleUI::promptForTaskIndex() {
    cout << t(TASK_INDEX_PROMPT);
    std::size_t index;
    cin >> index;
    cin.ignore();
    return index;
}

std::optional<MenuOption> ConsoleUI::toMenuOption(int value) {
    using enum MenuOption;
    switch (value) {
        case 1: return SHOW_TASKS;
        case 2: return ADD_TASK;
        case 3: return REMOVE_TASK;
        case 4: return MARK_DONE;
        case 5: return EXIT;
        default: return std::nullopt;
    }
}
