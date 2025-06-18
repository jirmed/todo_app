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
    void showMenu();
    void handleUserChoice();
    int getUserChoice();
};