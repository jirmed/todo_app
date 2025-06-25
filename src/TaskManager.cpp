#include "TaskManager.h"

#include <iostream>
#include <bits/ostream.tcc>

void TaskManager::addTask(std::string_view title) {
    tasks_.emplace_back(std::string(title));
    tasks_.back().id_ = nextId_++;
}


bool TaskManager::removeTask(std::size_t index) {
    if (index < 0 || index >= tasks_.size()) {
        return false;
    }
    tasks_.erase(tasks_.begin() + index);
    return true;
}

bool TaskManager::markDone(std::size_t index) {
    if (index < 0 || index >= tasks_.size()) {
        return false;
    }
    tasks_[index].done_ = true;
    return true;
}
