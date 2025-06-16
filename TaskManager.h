#pragma once
#include <vector>

#include "Task.h"


class TaskManager {
public:
    void addTask(std::string_view title);

    bool removeTask(int index) const;

    void printTasks();

private:
    std::vector<Task> tasks_;
};
