#include <gtest/gtest.h>
#include "http/RestServer.h"
#include "../../src/service/TaskManager.h"

class RestServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Inicializace před každým testem
    }
    
    void TearDown() override {
        // Cleanup po každém testu
    }
    
    TaskManager taskManager;
    RestServer restServer{taskManager};
};

TEST_F(RestServerTest, InitializesWithValidTaskManager) {
    // Test pouze ověří, že se objekt vytváří bez problémů
    // ReSharper disable once CppDeclaratorNeverUsed
    EXPECT_NO_THROW(RestServer server(taskManager));
}

TEST_F(RestServerTest, HandleGetAllTasks_ReturnsEmptyArrayWhenNoTasks) {
    // Test GET /tasks s prázdným seznamem
    const auto response = restServer.testHandleGetAllTasks();
    EXPECT_EQ(200, response.code);
    EXPECT_TRUE(response.body.find("[]") != std::string::npos);
}

TEST_F(RestServerTest, HandleGetAllTasks_ReturnsTasksWhenPresent) {
    // Přidáme úkol a otestujeme GET /tasks
    taskManager.addTask("Test Task");
    
    auto response = restServer.testHandleGetAllTasks();
    EXPECT_EQ(200, response.code);
    EXPECT_TRUE(response.body.find("Test Task") != std::string::npos);
}

TEST_F(RestServerTest, HandleAddTask_ValidJson_ReturnsCreated) {
    // Test POST /tasks s validním JSON
    crow::request req;
    req.body = R"({"title": "New Task"})";
    
    auto response = restServer.testHandleAddTask(req);
    EXPECT_EQ(201, response.code);
    EXPECT_TRUE(response.body.find("created") != std::string::npos);
}

TEST_F(RestServerTest, HandleAddTask_InvalidJson_ReturnsBadRequest) {
    // Test POST /tasks s nevalidním JSON
    crow::request req;
    req.body = "invalid json";
    
    auto response = restServer.testHandleAddTask(req);
    EXPECT_EQ(400, response.code);
}

TEST_F(RestServerTest, HandleRemoveTask_ValidId_ReturnsOk) {
    // Přidáme úkol a pak ho odstraníme
    taskManager.addTask("Task to Remove");
    const auto& tasks = taskManager.getAllTasks();
    ASSERT_FALSE(tasks.empty());

    const int taskId = static_cast<int>(tasks[0].id_);
    auto response = restServer.testHandleRemoveTask(taskId);
    EXPECT_EQ(200, response.code);
}

TEST_F(RestServerTest, HandleRemoveTask_InvalidId_ReturnsNotFound) {
    // Test odstranění neexistujícího úkolu
    const auto response = restServer.testHandleRemoveTask(999);
    EXPECT_EQ(404, response.code);
}

