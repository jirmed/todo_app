#include "TaskManager.h"

#include <iostream>
#include <bits/ostream.tcc>

void TaskManager::addTask(std::string_view title) {
    tasks_.emplace_back(std::string(title));
}


bool TaskManager::removeTask(std::size_t index) {
    if (index < 0 || index >= tasks_.size()) {
        return false;
    }
    tasks_.erase(tasks_.begin() + index);
    return true;
}

void TaskManager::printTasks() {
    std::cout << "Printing tasks" << std::endl;
    for (auto &task : tasks_) {
        std::cout << task.title_ << std::endl;
    }
}
