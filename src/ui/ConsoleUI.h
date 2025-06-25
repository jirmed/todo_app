#pragma once
#include "UI.h"
#include "TaskManager.h"
#include "Messages.h"

class ConsoleUI final : public UI {
public:
    explicit ConsoleUI(TaskManager& manager);
    void run() override;

    // Implementace čistých virtuálních metod z UI
    void notifySuccess(std::string_view message) override;
    void notifyInfo(std::string_view message) override;
    void notifyError(std::string_view message) override;
    void displayTasks(const std::vector<Task>& tasks) override;
    std::string promptForNewTaskTitle() override;
    std::size_t promptForTaskIndex() override;

private:
    TaskManager& manager_;
    
    void printMenu();
    int getMenuChoice();
    void handleShowTasks();
    void handleAddTask();
    void handleRemoveTask();
    void handleMarkDone();
    void printMessage(messages::MessageKey key);
};