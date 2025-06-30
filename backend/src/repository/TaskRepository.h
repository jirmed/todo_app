#pragma once

#include "model/Task.h"
#include <vector>
#include <string>
#include <optional>
#include <cstddef>

class TaskRepository {
public:
    virtual ~TaskRepository() = default;

    virtual Task addTask(std::string_view title) = 0;
    virtual bool markDoneById(std::size_t id) = 0;
    virtual bool removeTaskById(std::size_t id) = 0;
    virtual std::optional<Task> getTaskById(std::size_t id) const = 0;
    virtual std::vector<Task> getAllTasks() const = 0;
};
