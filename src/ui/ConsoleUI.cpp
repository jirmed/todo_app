#include "ConsoleUI.h"
#include <iostream>
#include <string>
#include <limits>
#ifdef _WIN32
#include <windows.h>
#endif

namespace {
    enum class MenuOption {
        SHOW_TASKS = 1,
        ADD_TASK = 2,
        REMOVE_TASK = 3,
        MARK_DONE = 4,
        EXIT = 5
    };

    class UIMessages {
    public:
        // Success messages
        static constexpr auto TASK_ADDED = "Úkol byl přidán!";
        static constexpr auto TASK_REMOVED = "Úkol byl odstraněn!";
        static constexpr auto TASK_COMPLETED = "Úkol byl označen jako dokončený!";

        // Error messages
        static constexpr auto INVALID_TASK_NUMBER = "Neplatné číslo úkolu!";
        static constexpr auto INVALID_CHOICE = "Neplatná volba! Zkuste to znovu.";

        // Info messages
        static constexpr auto NO_TASKS = "Žádné úkoly.";
        static constexpr auto APP_TITLE = "=== TODO APLIKACE ===";
        static constexpr auto MENU_TITLE = "\n--- MENU ---";
        static constexpr auto EXITING = "Ukončuji aplikaci...";
        static constexpr auto TASKS_LIST_TITLE = "\nSeznam úkolů:";

        // Prompts
        static constexpr auto MENU_PROMPT = "Vyberte možnost (1-5): ";
        static constexpr auto TASK_TITLE_PROMPT = "Zadejte název úkolu: ";
        static constexpr auto TASK_INDEX_PROMPT = "Zadejte číslo úkolu: ";

        // Menu options
        static constexpr auto MENU_SHOW_TASKS = "1. Zobrazit úkoly";
        static constexpr auto MENU_ADD_TASK = "2. Přidat úkol";
        static constexpr auto MENU_REMOVE_TASK = "3. Odstranit úkol";
        static constexpr auto MENU_MARK_DONE = "4. Označit úkol jako dokončený";
        static constexpr auto MENU_EXIT = "5. Ukončit";
    };
}

ConsoleUI::ConsoleUI(TaskManager &manager) : manager_(manager) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void ConsoleUI::run() {
    std::cout << UIMessages::APP_TITLE << "\n";
    while (true) {
        showMenu();
        handleUserChoice();
    }
}

void ConsoleUI::showMenu() {
    std::cout << UIMessages::MENU_TITLE << "\n";
    std::cout << UIMessages::MENU_SHOW_TASKS << "\n";
    std::cout << UIMessages::MENU_ADD_TASK << "\n";
    std::cout << UIMessages::MENU_REMOVE_TASK << "\n";
    std::cout << UIMessages::MENU_MARK_DONE << "\n";
    std::cout << UIMessages::MENU_EXIT << "\n";
    std::cout << UIMessages::MENU_PROMPT;
}

void ConsoleUI::handleUserChoice() {
    switch (static_cast<MenuOption>(getUserChoice())) {
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
}

void ConsoleUI::handleAddTask() {
    manager_.addTask(promptForNewTaskTitle());
    notifySuccess(UIMessages::TASK_ADDED);
}

void ConsoleUI::handleRemoveTask() {
    if (manager_.removeTask(promptForTaskIndex())) {
        notifySuccess(UIMessages::TASK_REMOVED);
    } else {
        notifyError(UIMessages::INVALID_TASK_NUMBER);
    }
}

void ConsoleUI::handleMarkDone() {
    if (manager_.markDone(promptForTaskIndex())) {
        notifySuccess(UIMessages::TASK_COMPLETED);
    } else {
        notifyError(UIMessages::INVALID_TASK_NUMBER);
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
    std::cout << UIMessages::EXITING << "\n";
    std::exit(0);
}

void ConsoleUI::showInvalidChoiceMessage() {
    std::cout << UIMessages::INVALID_CHOICE << "\n";
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
        std::cout << UIMessages::NO_TASKS << "\n";
        return;
    }

    std::cout << UIMessages::TASKS_LIST_TITLE << "\n";
    for (std::size_t i = 0; i < tasks.size(); ++i) {
        std::cout << formatTaskItem(i, tasks[i]) << "\n";
    }
}

std::string ConsoleUI::formatTaskItem(std::size_t index, const Task &task) {
    const char statusChar = task.done_ ? 'x' : ' ';
    return std::to_string(index) + ". [" + statusChar + "] " + task.title_;
}

std::string ConsoleUI::promptForNewTaskTitle() {
    std::cout << UIMessages::TASK_TITLE_PROMPT;
    std::string title;
    std::getline(std::cin >> std::ws, title);
    return title;
}

std::size_t ConsoleUI::promptForTaskIndex() {
    std::cout << UIMessages::TASK_INDEX_PROMPT;
    std::size_t index;
    std::cin >> index;
    std::cin.ignore();
    return index;
}