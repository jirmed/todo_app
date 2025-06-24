// RestServer.h
#pragma once
#include "TaskManager.h"

class RestServer {
public:
    explicit RestServer(TaskManager& manager);
    void run();
private:
    TaskManager& manager_;
};
