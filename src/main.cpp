#include <iostream>
#include <string>
#include "TaskManager.h"

using std::cout;
using std::endl;
using std::string;
using std::cin;

const int EXIT_CHOICE = 4;
const int INPUT_BUFFER_SIZE = 10000;

void displayMenu() {
    cout << "Welcome to the task manager" << endl;
    cout << "1. Add task" << endl;
    cout << "2. Remove task" << endl;
    cout << "3. Print tasks" << endl;
    cout << "4. Exit" << endl << endl;
    cout << "Enter your choice: ";
}

void handleInvalidInput() {
    cin.clear();
    cin.ignore(INPUT_BUFFER_SIZE, '\n');
    cout << "Invalid input. Please try again.\n";
}

void handleAddTask(TaskManager &manager) {
    cout << "Enter task title: ";
    string title;
    cin.ignore();
    getline(cin, title);
    manager.addTask(title);
}

void handleRemoveTask(TaskManager &manager) {
    cout << "Enter task index: ";
    int index;
    cin >> index;
    manager.removeTask(index);
}

int main() {
    TaskManager manager;
    int choice = 0;

    while (choice != EXIT_CHOICE) {
        displayMenu();

        if (!(cin >> choice)) {
            handleInvalidInput();
            continue;
        }

        switch (choice) {
            case 1:
                handleAddTask(manager);
                break;
            case 2:
                handleRemoveTask(manager);
                break;
            case 3:
                manager.printTasks();
                break;
            case EXIT_CHOICE:
                break;
            default:
                cout << "Invalid choice. Please try again.\n\n";
        }
    }

    return 0;
}