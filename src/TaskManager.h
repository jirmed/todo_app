#pragma once
#include <vector>
#include <string_view>
#include "Task.h"

class TaskManager {
public:
    void addTask(std::string_view title);
    bool removeTask(std::size_t index);
    bool markDone(std::size_t index);

    bool isValidIndex(std::size_t index) const;

    [[nodiscard]] const std::vector<Task>& getAllTasks() const {
        return tasks_;
    }


private:
    std::vector<Task> tasks_;
    std::size_t nextId_ = 1;
};
