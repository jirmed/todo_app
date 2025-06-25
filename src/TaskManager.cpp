#include "TaskManager.h"
#include <iostream>
#include <bits/ostream.tcc>
#include <algorithm>

void TaskManager::addTask(std::string_view title) {
    tasks_.emplace_back(std::string(title));
    tasks_.back().id_ = nextId_++;
}

bool TaskManager::removeTask(std::size_t index) {
    if (!isValidIndex(index)) {
        return false;
    }
    tasks_.erase(tasks_.begin() + index);
    return true;
}

bool TaskManager::markDone(std::size_t index) {
    if (!isValidIndex(index)) {
        return false;
    }
    tasks_[index].done_ = true;
    return true;
}

bool TaskManager::markDoneById(std::size_t id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(), 
                          [id](const Task& task) { return task.id_ == id; });
    
    if (it != tasks_.end()) {
        it->done_ = true;
        return true;
    }
    return false;
}

bool TaskManager::removeTaskById(std::size_t id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(), 
                          [id](const Task& task) { return task.id_ == id; });
    
    if (it != tasks_.end()) {
        tasks_.erase(it);
        return true;
    }
    return false;
}

bool TaskManager::isValidIndex(std::size_t index) const {
    return index < tasks_.size();
}