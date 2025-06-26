// FileRepository.h
#pragma once

#include "model/TaskRepository.h"
#include "repository/dso/TaskDSO.h"
#include <string>
#include <vector>

class FileRepository final : public TaskRepository {
public:
    explicit FileRepository(const std::string &filePath);

    Task addTask(std::string_view title) override;

    bool markDoneById(std::size_t id) override;

    bool removeTaskById(std::size_t id) override;

    std::optional<Task> getTaskById(std::size_t id) const override;

    std::vector<Task> getAllTasks() const override;

private:
    void loadFromFile();

    void saveToFile() const;

    std::size_t generateNextId() const;

    std::vector<TaskDSO>::iterator findTaskById(std::size_t id);

    std::vector<TaskDSO>::const_iterator findTaskById(std::size_t id) const;

    std::string filePath_;
    std::vector<TaskDSO> taskDSOs_;
};