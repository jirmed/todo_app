#pragma once
#include <vector>
#include <string_view>
#include "Task.h"

class TaskManager {
public:
    void addTask(std::string_view title);
    bool markDoneById(std::size_t id);
    bool removeTaskById(std::size_t id);

    bool isValidIndex(std::size_t index) const;

    [[nodiscard]] const std::vector<Task>& getAllTasks() const {
        return tasks_;
    }

private:
    std::vector<Task> tasks_;
    std::size_t nextId_ = 1;
};