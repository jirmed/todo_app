#include "TaskManager.h"

#include <iostream>
#include <bits/ostream.tcc>

void TaskManager::addTask(const std::string_view title) {
    tasks_.emplace_back(std::string(title));
}


bool TaskManager::removeTask(const int index) const {
    if (index < 0 || index >= tasks_.size()) {
        return false;
    }
    return false;

}

void TaskManager::printTasks() {
    std::cout << "Printing tasks" << std::endl;
    for (auto &task : tasks_) {
        std::cout << task.title_ << std::endl;
    }
}
