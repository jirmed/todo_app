#pragma once

#include <string>
#include <cstddef>
#include "model/Task.h"
#include "model/Task.h"

// Data Store Object pro persistenci Task objektů
struct TaskDSO {
    std::size_t id;
    std::string title;
    bool done;

    // Konverze z doménového modelu na DSO
    static TaskDSO fromTask(const Task& task) {
        return {task.id_, task.title_, task.done_};
    }

    // Konverze z DSO na doménový model
    Task toTask() const {
        return {id, title, done};
    }
};

