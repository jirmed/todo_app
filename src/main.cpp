#include <iostream>
#include <string>
#include "TaskManager.h"

int main() {
    TaskManager manager;
    int choice;
    std::string taskTitle;
    size_t index;

    while (true) {
        std::cout << "\nTodo App Menu:\n";
        std::cout << "1. List tasks\n";
        std::cout << "2. Add task\n";
        std::cout << "3. Remove task\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please try again.\n";
            continue;
        }

        switch (choice) {
            case 1:
                manager.printTasks();
                break;
            case 2:
                std::cin.ignore();
                std::cout << "Enter task title: ";
                std::getline(std::cin, taskTitle);
                manager.addTask(taskTitle);
                std::cout << "Task added successfully!\n";
                break;
            case 3:
                std::cout << "Enter task index to remove: ";
                std::cin >> index;
                if (manager.removeTask(index)) {
                    std::cout << "Task removed successfully!\n";
                } else {
                    std::cout << "Invalid task index!\n";
                }
                break;
            case 4:
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}