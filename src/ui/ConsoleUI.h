#pragma once
#include "UI.h"
#include "../TaskManager.h"
#include <memory>
#include <optional>

enum class MenuOption {
    SHOW_TASKS = 1,
    ADD_TASK = 2,
    REMOVE_TASK = 3,
    MARK_DONE = 4,
    EXIT = 5
};

class ConsoleUI final : public UI {
private:
    TaskManager &manager_;

public:
    explicit ConsoleUI(TaskManager &manager);

    void run() override;

    // UI interface overrides
    void notifySuccess(std::string_view message) override;

    void notifyInfo(std::string_view message) override;

    void notifyError(std::string_view message) override;

    void displayTasks(const std::vector<Task> &tasks) override;

    std::string formatTaskItem(std::size_t index, const Task &task);

    std::string promptForNewTaskTitle() override;

    std::size_t promptForTaskIndex() override;

    std::optional<MenuOption> toMenuOption(int value);

private:
    static void showMenu();

    void handleUserChoice();

    void handleAddTask();

    void handleRemoveTask();

    void handleMarkDone();

    static int getUserChoice();

    static void exitApplication();

    static void showInvalidChoiceMessage();
};