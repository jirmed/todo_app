#pragma once
#include "UI.h"
#include "../TaskManager.h"
#include <memory>

class ConsoleUI : public UI {
private:
    TaskManager& manager_;

public:
    explicit ConsoleUI(TaskManager& manager);
    
    void run() override;
    
private:
    static void showMenu();
    void handleUserChoice() const;

    static int getUserChoice();
    void showTasks() const;
    void addNewTask() const;
    void removeExistingTask() const;

    static void exitApplication();

    static void showInvalidChoiceMessage();
};