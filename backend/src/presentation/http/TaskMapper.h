#pragma once
#include "model/Task.h"
#include "dto/TaskDto.h"

struct TaskMapper {
    static TaskDto toDto(const Task& task) {
        return TaskDto{static_cast<int>(task.id_), task.title_, task.done_};
    }

    static Task fromDto(const TaskDto& dto) {
        // Použijeme existující konstruktor s ID, názvem a stavem dokončení
        return Task(dto.id, dto.title, dto.done);
    }
};