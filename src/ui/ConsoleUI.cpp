#include "ConsoleUI.h"
#include <iostream>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif

ConsoleUI::ConsoleUI(TaskManager& manager) : manager_(manager) {
#ifdef _WIN32
    // Nastavení UTF-8 pro Windows
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    // Nepoužíváme _O_U8TEXT, protože může způsobit problémy s std::cout
    // _setmode(_fileno(stdout), _O_U8TEXT);
    // _setmode(_fileno(stdin), _O_U8TEXT);
#endif
}

void ConsoleUI::run() {
    printMessage(messages::MessageKey::APP_TITLE);
    
    int choice;
    do {
        printMenu();
        choice = getMenuChoice();
        
        switch (choice) {
            case 1:
                handleShowTasks();
                break;
            case 2:
                handleAddTask();
                break;
            case 3:
                handleRemoveTask();
                break;
            case 4:
                handleMarkDone();
                break;
            case 5:
                printMessage(messages::MessageKey::EXITING);
                break;
            default:
                printMessage(messages::MessageKey::INVALID_CHOICE);
                break;
        }
    } while (choice != 5);
}

void ConsoleUI::notifySuccess(std::string_view message) {
    std::cout << "[ÚSPĚCH] " << message << std::endl;
}

void ConsoleUI::notifyInfo(std::string_view message) {
    std::cout << "[INFO] " << message << std::endl;
}

void ConsoleUI::notifyError(std::string_view message) {
    std::cout << "[ERROR] " << message << std::endl;
}

void ConsoleUI::displayTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        printMessage(messages::MessageKey::NO_TASKS);
        return;
    }
    
    printMessage(messages::MessageKey::TASKS_LIST_TITLE);
    for (const auto& task : tasks) {
        std::cout << "ID: " << task.id_ << " - " << task.title_;
        if (task.done_) {
            std::cout << " [COMPLETED]";
        }
        std::cout << std::endl;
    }
}

std::string ConsoleUI::promptForNewTaskTitle() {
    std::string title;
    std::cout << messages::get(messages::MessageKey::TASK_TITLE_PROMPT);
    std::getline(std::cin, title);
    return title;
}

std::size_t ConsoleUI::promptForTaskIndex() {
    std::size_t taskId;
    std::cout << messages::get(messages::MessageKey::TASK_INDEX_PROMPT);
    
    while (!(std::cin >> taskId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        notifyError("Neplatné ID úkolu!");
        std::cout << messages::get(messages::MessageKey::TASK_INDEX_PROMPT);
    }
    
    std::cin.ignore(); // Vyčistit buffer
    return taskId;
}

void ConsoleUI::printMenu() {
    printMessage(messages::MessageKey::MENU_TITLE);
    printMessage(messages::MessageKey::MENU_SHOW_TASKS);
    printMessage(messages::MessageKey::MENU_ADD_TASK);
    printMessage(messages::MessageKey::MENU_REMOVE_TASK);
    printMessage(messages::MessageKey::MENU_MARK_DONE);
    printMessage(messages::MessageKey::MENU_EXIT);
}

int ConsoleUI::getMenuChoice() {
    int choice;
    std::cout << messages::get(messages::MessageKey::MENU_PROMPT);
    
    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printMessage(messages::MessageKey::INVALID_CHOICE);
        std::cout << messages::get(messages::MessageKey::MENU_PROMPT);
    }
    
    std::cin.ignore(); // Vyčistit buffer
    return choice;
}

void ConsoleUI::handleShowTasks() {
    const auto& tasks = manager_.getAllTasks();
    displayTasks(tasks);
}

void ConsoleUI::handleAddTask() {
    std::string title = promptForNewTaskTitle();
    
    if (!title.empty()) {
        manager_.addTask(title);
        notifySuccess(messages::get(messages::MessageKey::TASK_ADDED));
    }
}

void ConsoleUI::handleRemoveTask() {
    const auto& tasks = manager_.getAllTasks();
    if (tasks.empty()) {
        notifyInfo(messages::get(messages::MessageKey::NO_TASKS));
        return;
    }
    
    displayTasks(tasks); // Zobrazí úkoly s ID
    
    std::size_t taskId = promptForTaskIndex();
    
    if (manager_.removeTaskById(taskId)) {
        notifySuccess(messages::get(messages::MessageKey::TASK_REMOVED));
    } else {
        notifyError(messages::get(messages::MessageKey::INVALID_TASK_NUMBER));
    }
}

void ConsoleUI::handleMarkDone() {
    const auto& tasks = manager_.getAllTasks();
    if (tasks.empty()) {
        notifyInfo(messages::get(messages::MessageKey::NO_TASKS));
        return;
    }
    
    displayTasks(tasks); // Zobrazí úkoly s ID
    
    std::size_t taskId = promptForTaskIndex();
    
    if (manager_.markDoneById(taskId)) {
        notifySuccess(messages::get(messages::MessageKey::TASK_COMPLETED));
    } else {
        notifyError(messages::get(messages::MessageKey::INVALID_TASK_NUMBER));
    }
}

void ConsoleUI::printMessage(messages::MessageKey key) {
    std::cout << messages::get(key) << std::endl;
}