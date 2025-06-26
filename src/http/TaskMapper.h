#pragma once
#include "../model/Task.h"
#include "dto/TaskDto.h"

struct TaskMapper {
    static TaskDto toDto(const Task& task) {
        return TaskDto{task.id_, task.title_, task.done_};
    }

    static Task fromDto(const TaskDto& dto) {
        return Task(dto.title, dto.done);
    }
};
