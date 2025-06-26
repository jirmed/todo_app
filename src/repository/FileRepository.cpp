#include "FileRepository.h"
#include "repository/dso/TaskDSO.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

namespace {
    constexpr char FIELD_SEPARATOR = ',';
    constexpr char TASK_DONE_MARKER = '1';
    constexpr char TASK_NOT_DONE_MARKER = '0';
}

FileRepository::FileRepository(const std::string &filePath) : filePath_(filePath) {
    loadFromFile();
}

std::vector<Task> FileRepository::getAllTasks() const {
    std::vector<Task> result;
    result.reserve(taskDSOs_.size());

    std::transform(taskDSOs_.begin(), taskDSOs_.end(),
                   std::back_inserter(result),
                   [](const TaskDSO &dso) { return dso.toTask(); });

    return result;
}

Task FileRepository::addTask(std::string_view title) {
    Task task{std::string(title)};
    task.id_ = generateNextId();

    TaskDSO dso = TaskDSO::fromTask(task);
    taskDSOs_.push_back(dso);
    saveToFile();

    return task;
}

bool FileRepository::removeTaskById(size_t id) {
    auto taskIter = findTaskById(id);
    if (taskIter == taskDSOs_.end()) {
        return false;
    }

    taskDSOs_.erase(taskIter);
    saveToFile();
    return true;
}

bool FileRepository::markDoneById(size_t id) {
    auto taskIter = findTaskById(id);
    if (taskIter == taskDSOs_.end()) {
        return false;
    }

    taskIter->done = true;
    saveToFile();
    return true;
}

std::optional<Task> FileRepository::getTaskById(std::size_t id) const {
    auto taskIter = findTaskById(id);
    if (taskIter == taskDSOs_.end()) {
        return std::nullopt;
    }

    return taskIter->toTask();
}

void FileRepository::loadFromFile() {
    std::ifstream file(filePath_);
    if (!file.is_open()) {
        std::cerr << "Nelze otevřít soubor pro čtení: " << filePath_ << std::endl;
        return;
    }

    taskDSOs_.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, title, done;

        if (std::getline(ss, id, FIELD_SEPARATOR) &&
            std::getline(ss, title, FIELD_SEPARATOR) &&
            std::getline(ss, done, FIELD_SEPARATOR)) {
            TaskDSO dso{
                std::stoull(id),
                title,
                done == std::string(1, TASK_DONE_MARKER)
            };

            taskDSOs_.push_back(dso);
        }
    }
}

void FileRepository::saveToFile() const {
    std::ofstream file(filePath_);
    if (!file.is_open()) {
        std::cerr << "Nelze otevřít soubor pro zápis: " << filePath_ << std::endl;
        return;
    }

    for (const auto &dso: taskDSOs_) {
        file << dso.id << FIELD_SEPARATOR
                << dso.title << FIELD_SEPARATOR
                << (dso.done ? TASK_DONE_MARKER : TASK_NOT_DONE_MARKER) << '\n';
    }
}

std::size_t FileRepository::generateNextId() const {
    std::size_t maxId = 0;
    for (const auto &dso: taskDSOs_) {
        maxId = std::max(maxId, dso.id);
    }
    return maxId + 1;
}

std::vector<TaskDSO>::iterator FileRepository::findTaskById(std::size_t id) {
    return std::find_if(taskDSOs_.begin(), taskDSOs_.end(),
                        [id](const TaskDSO &dso) { return dso.id == id; });
}

std::vector<TaskDSO>::const_iterator FileRepository::findTaskById(std::size_t id) const {
    return std::find_if(taskDSOs_.begin(), taskDSOs_.end(),
                        [id](const TaskDSO &dso) { return dso.id == id; });
}