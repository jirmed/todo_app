#include <iostream>

#include "TaskManager.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    TaskManager manager;


    manager.addTask("My first task");
    manager.printTasks();
    return 0;
}