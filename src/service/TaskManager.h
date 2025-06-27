#pragma once

#include "model/Task.h"
#include "repository/TaskRepository.h"
#include <vector>
#include <string>
#include <memory> // Pro std::unique_ptr

class TaskManager {
public:
    // TaskManager nyní přijímá unique_ptr na ITaskRepository
    explicit TaskManager(std::unique_ptr<TaskRepository> repository);

    void addTask(std::string_view title) const;
    bool markDoneById(std::size_t id) const;
    bool removeTaskById(std::size_t id) const;
    bool isValidId(std::size_t id) const; // Upraveno pro kontrolu ID, ne indexu
    std::vector<Task> getAllTasks() const; // Nová metoda pro získání všech úkolů

private:
    std::unique_ptr<TaskRepository> repository_;
};