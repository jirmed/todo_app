#pragma once
#include "UI.h"
#include "../TaskManager.h"
#include <memory>

class ConsoleUI : public UI {
private:
    TaskManager &manager_;

public:
    explicit ConsoleUI(TaskManager &manager);

    void run() override;

    // UI interface overrides
    void notifySuccess(const std::string &message) override;

    void notifyInfo(const std::string &message) override;

    void notifyError(const std::string &message) override;

    void displayTasks(const std::vector<Task> &tasks) override;

    std::string promptForNewTaskTitle() override;

    std::size_t promptForTaskIndex() override;

private:
    static void showMenu();

    void handleUserChoice();

    static int getUserChoice();

    static void exitApplication();

    static void showInvalidChoiceMessage();
};
