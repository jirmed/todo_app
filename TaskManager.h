#pragma once
#include <vector>

#include "Task.h"


class TaskManager {
public:
    void addTask(std::string_view title);

    bool removeTask(std::size_t index);

    void printTasks();

private:
    std::vector<Task> tasks_;

public:
    [[nodiscard]] std::vector<Task> tasks() const {
        return tasks_;
    }
};
