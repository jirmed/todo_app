//
// Created by jiri21 on 17.06.2025.
//

#include "ConsoleUI.h"
#include <iostream>
#include <string>

ConsoleUI::ConsoleUI(TaskManager& manager) : manager_(manager) {}

void ConsoleUI::run() {
    std::cout << "=== TODO APLIKACE ===" << std::endl;
    
    while (true) {
        showMenu();
        handleUserChoice();
    }
}

void ConsoleUI::showMenu() {
    std::cout << "\n--- MENU ---" << std::endl;
    std::cout << "1. Zobrazit úkoly" << std::endl;
    std::cout << "2. Přidat úkol" << std::endl;
    std::cout << "3. Odstranit úkol" << std::endl;
    std::cout << "4. Ukončit" << std::endl;
    std::cout << "Vyberte možnost (1-4): ";
}

void ConsoleUI::handleUserChoice() {
    int choice = getUserChoice();
    
    switch (choice) {
        case 1:
            manager_.printTasks();
            break;
            
        case 2: {
            std::cout << "Zadejte název úkolu: ";
            std::string title;
            std::getline(std::cin, title);
            manager_.addTask(title);
            std::cout << "Úkol byl přidán!" << std::endl;
            break;
        }
        
        case 3: {
            manager_.printTasks();
            std::cout << "Zadejte číslo úkolu k odstranění (začínáme od 0): ";
            size_t index;
            std::cin >> index;
            std::cin.ignore(); // Vyčistit buffer
            
            if (manager_.removeTask(index)) {
                std::cout << "Úkol byl odstraněn!" << std::endl;
            } else {
                std::cout << "Neplatné číslo úkolu!" << std::endl;
            }
            break;
        }
        
        case 4:
            std::cout << "Ukončuji aplikaci..." << std::endl;
            exit(0);
            
        default:
            std::cout << "Neplatná volba! Zkuste to znovu." << std::endl;
            break;
    }
}

int ConsoleUI::getUserChoice() {
    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Vyčistit buffer po čtení čísla
    return choice;
}