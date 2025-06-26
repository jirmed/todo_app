#pragma once

#include <string>
#include <vector>
#include "../model/Task.h"

class UI {
public:
    virtual ~UI() = default;

    [[noreturn]] virtual void run() = 0;

    // Informace o úspěchu či obecná informace
    virtual void notifySuccess(std::string_view message) = 0;
    virtual void notifyInfo(std::string_view message) = 0;

    // Hlásit chybu
    virtual void notifyError(std::string_view message) = 0;

    // Výpis seznamu úkolů
    virtual void displayTasks(const std::vector<Task>& tasks) = 0;

    // Interakce s uživatelem (v konzoli) nebo zpracování požadavku (REST)
    virtual std::string promptForNewTaskTitle() = 0;
    virtual std::size_t promptForTaskIndex() = 0;
};
