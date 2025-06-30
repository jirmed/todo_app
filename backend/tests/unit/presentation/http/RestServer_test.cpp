#include <gtest/gtest.h>
#include "presentation/http/RestServer.h"
#include "service/TaskManager.h"
#include <repository/memory/InMemoryTaskRepository.h>
#include <nlohmann/json.hpp>
#include <memory>

class RestServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        taskManager = std::make_unique<TaskManager>(std::make_unique<InMemoryTaskRepository>());
        restServer = std::make_unique<RestServer>(*taskManager);
    }

    void TearDown() override {
        restServer.reset();
        taskManager.reset();
    }

    std::unique_ptr<TaskManager> taskManager;
    std::unique_ptr<RestServer> restServer;
};

TEST_F(RestServerTest, InitializesWithValidTaskManager) {
    ASSERT_NE(taskManager, nullptr);
    ASSERT_NE(restServer, nullptr);
}

TEST_F(RestServerTest, HandleGetAllTasks_ReturnsEmptyArrayWhenNoTasks) {
    // Voláme přímo veřejnou metodu handleGetAllTasks
    const auto response = restServer->handleGetAllTasks();
    EXPECT_EQ(200, response.code);
    EXPECT_TRUE(response.body.find("[]") != std::string::npos);
}

TEST_F(RestServerTest, HandleGetAllTasks_ReturnsTasksWhenPresent) {
    taskManager->addTask("Test Task");
    // Voláme přímo veřejnou metodu handleGetAllTasks
    auto response = restServer->handleGetAllTasks();
    EXPECT_EQ(200, response.code);
    EXPECT_TRUE(response.body.find("Test Task") != std::string::npos);
}

TEST_F(RestServerTest, HandleAddTask_ValidJson_ReturnsCreated) {
    crow::request req;
    req.body = R"({"title": "New Task"})";
    // Voláme přímo veřejnou metodu handleAddTask
    auto response = restServer->handleAddTask(req);
    EXPECT_EQ(201, response.code);
    const auto jsonBody = nlohmann::json::parse(response.body);
    EXPECT_EQ("New Task", jsonBody["title"]);
    EXPECT_FALSE(jsonBody["done"]);
}

TEST_F(RestServerTest, HandleAddTask_InvalidJson_ReturnsBadRequest) {
    crow::request req;
    req.body = "invalid json";
    // Voláme přímo veřejnou metodu handleAddTask
    auto response = restServer->handleAddTask(req);
    EXPECT_EQ(400, response.code);
}

TEST_F(RestServerTest, HandleRemoveTask_ValidId_ReturnsOk) {
    Task createdTask = taskManager->addTask("Task to Remove");
    const int taskId = static_cast<int>(createdTask.id_);
    // Voláme přímo veřejnou metodu handleRemoveTask
    auto response = restServer->handleRemoveTask(taskId);
    EXPECT_EQ(200, response.code);
}

TEST_F(RestServerTest, HandleRemoveTask_InvalidId_ReturnsNotFound) {
    // Voláme přímo veřejnou metodu handleRemoveTask
    const auto response = restServer->handleRemoveTask(999);
    EXPECT_EQ(404, response.code);
}