#include "TaskManager.h"
#include <iostream>
#include <algorithm> // Může být odstraněno, protože logiku přesouváme do repository

TaskManager::TaskManager(std::unique_ptr<TaskRepository> repository)
    : repository_(std::move(repository)) {}

Task TaskManager::addTask(std::string_view title) const {
    auto task = repository_->addTask(title);   // Předpokládáme, že repo vrací id nového tasku
    return task;
}

bool TaskManager::markDoneById(std::size_t id) const {
    return repository_->markDoneById(id);
}

bool TaskManager::removeTaskById(std::size_t id) const {
    return repository_->removeTaskById(id);
}

// Tato metoda nyní kontroluje existenci úkolu podle ID v repozitáři
bool TaskManager::isValidId(std::size_t id) const {
    return repository_->getTaskById(id).has_value();
}

std::vector<Task> TaskManager::getAllTasks() const {
    return repository_->getAllTasks();
}