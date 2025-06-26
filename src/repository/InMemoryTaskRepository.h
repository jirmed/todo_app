#pragma once

#include "../model/TaskRepository.h"
#include <vector>
#include <string>
#include <algorithm> // Pro std::ranges::find_if
#include <cstddef>   // Pro std::size_t
#include <optional>

class InMemoryTaskRepository final : public TaskRepository {
public:
    Task addTask(std::string_view title) override;
    bool markDoneById(std::size_t id) override;
    bool removeTaskById(std::size_t id) override;
    std::optional<Task> getTaskById(std::size_t id) const override;
    std::vector<Task> getAllTasks() const override;

private:
    std::vector<Task> tasks_;
    std::size_t nextId_ = 1;
};
