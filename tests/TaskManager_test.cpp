#include <gtest/gtest.h>
#include "../TaskManager.h"

#include <iostream>

class TaskManagerTest : public ::testing::Test {
protected:
    TaskManager taskManager;
};

TEST_F(TaskManagerTest, AddTask_AddsTaskToCollection) {
    taskManager.addTask("Test Task");

    EXPECT_EQ(1, taskManager.tasks().size());
}

TEST_F(TaskManagerTest, RemoveTask_ValidIndex_RemovesTaskAndReturnsTrue) {
    taskManager.addTask("Task 1");
    taskManager.addTask("Task 2");

    bool result = taskManager.removeTask(0);

    EXPECT_TRUE(result);
    EXPECT_EQ(1, taskManager.tasks().size());

}

TEST_F(TaskManagerTest, RemoveTask_InvalidIndex_ReturnsFalse) {
    taskManager.addTask("Task 1");

    bool result = taskManager.removeTask(999);

    EXPECT_FALSE(result);
    EXPECT_EQ(1, taskManager.tasks().size());
}
