#include <iostream>
#include <string>
#include "TaskManager.h"

using std::cout;
using std::endl;
using std::string;
using std::cin;

int main() {
    TaskManager manager;
    int choice = 0;

    while (choice != 4) {
        cout << "Welcome to the task manager" << endl;
        cout << "1. Add task" << endl;
        cout << "2. Remove task" << endl;
        cout << "3. Print tasks" << endl;
        cout << "4. Exit" << endl << endl;
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please try again.\n";
            continue;
        }
        switch (choice) {
            case 1: {
                cout << "Enter task title: ";
                string title;
                cin.ignore(); // vyčistit buffer
                getline(cin, title);
                manager.addTask(title);
                break;
            }
            case 2: {
                cout << "Enter task index: ";
                int index;
                if (!(cin >> index)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Please enter a number.\n";
                    break;
                }
                if (!manager.removeTask(index)) {
                    cout << "Invalid index.\n";
                }
                break;
            }
            case 3: {
                manager.printTasks();
                break;
            }
            case 4:
                break;
            default:
                cout << "Invalid choice. Please try again.\n\n";
        }
    }


    return
            0;
}
