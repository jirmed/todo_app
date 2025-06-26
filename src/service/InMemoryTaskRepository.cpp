#include "InMemoryTaskRepository.h"
#include <algorithm> // Pro std::ranges::find_if

Task InMemoryTaskRepository::addTask(std::string_view title) {
    Task newTask{std::string(title)};
    newTask.id_ = nextId_++;
    tasks_.push_back(newTask);
    return newTask;
}

bool InMemoryTaskRepository::markDoneById(std::size_t id) {
    const auto it = std::ranges::find_if(tasks_,
                                         [id](const Task& task) { return task.id_ == id; });

    if (it != tasks_.end()) {
        it->done_ = true;
        return true;
    }
    return false;
}

bool InMemoryTaskRepository::removeTaskById(std::size_t id) {
    const auto it = std::ranges::find_if(tasks_,
                                         [id](const Task& task) { return task.id_ == id; });

    if (it != tasks_.end()) {
        tasks_.erase(it);
        return true;
    }
    return false;
}

std::optional<Task> InMemoryTaskRepository::getTaskById(std::size_t id) const {
    const auto it = std::ranges::find_if(tasks_,
                                         [id](const Task& task) { return task.id_ == id; });
    if (it != tasks_.end()) {
        return *it;
    }
    return std::nullopt;
}

std::vector<Task> InMemoryTaskRepository::getAllTasks() const {
    return tasks_;
}