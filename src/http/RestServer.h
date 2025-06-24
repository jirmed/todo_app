#include "TaskManager.h"

class RestServer {
private:
    TaskManager manager_;

public:
    RestServer(TaskManager& manager_);
    void run();
};
