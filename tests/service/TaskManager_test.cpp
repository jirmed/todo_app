#include <gtest/gtest.h>
#include "../../src/service/TaskManager.h"

class TaskManagerTest : public ::testing::Test {
protected:
    TaskManager taskManager;
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST_F(TaskManagerTest, AddTask_AddsTaskToCollection) {
    taskManager.addTask("Test Task");

    EXPECT_EQ(1, taskManager.getAllTasks().size());
}

TEST_F(TaskManagerTest, RemoveTaskById_ValidId_RemovesTaskAndReturnsTrue) {
    taskManager.addTask("Task 1");
    taskManager.addTask("Task 2");
    
    // Získáme ID první úlohy
    const auto& tasks = taskManager.getAllTasks();
    ASSERT_FALSE(tasks.empty());
    std::size_t firstTaskId = tasks[0].id_;

    bool result = taskManager.removeTaskById(firstTaskId);

    EXPECT_TRUE(result);
    EXPECT_EQ(1, taskManager.getAllTasks().size());
}

TEST_F(TaskManagerTest, RemoveTaskById_InvalidId_ReturnsFalse) {
    taskManager.addTask("Task 1");

    bool result = taskManager.removeTaskById(999); // Neexistující ID

    EXPECT_FALSE(result);
    EXPECT_EQ(1, taskManager.getAllTasks().size());
}

TEST_F(TaskManagerTest, MarkDoneById_ValidId_MarksTaskAndReturnsTrue) {
    taskManager.addTask("Task 1");
    
    // Získáme ID úlohy
    const auto& tasks = taskManager.getAllTasks();
    ASSERT_FALSE(tasks.empty());
    std::size_t taskId = tasks[0].id_;
    
    // Ověříme, že úloha není označená jako dokončená
    EXPECT_FALSE(tasks[0].done_);

    bool result = taskManager.markDoneById(taskId);

    EXPECT_TRUE(result);
    
    // Ověříme, že úloha je nyní označená jako dokončená
    const auto& updatedTasks = taskManager.getAllTasks();
    EXPECT_TRUE(updatedTasks[0].done_);
}

TEST_F(TaskManagerTest, MarkDoneById_InvalidId_ReturnsFalse) {
    taskManager.addTask("Task 1");

    bool result = taskManager.markDoneById(999); // Neexistující ID

    EXPECT_FALSE(result);
    
    // Ověříme, že úloha zůstala neměnná
    const auto& tasks = taskManager.getAllTasks();
    EXPECT_FALSE(tasks[0].done_);
}

TEST_F(TaskManagerTest, TasksHaveUniqueIds) {
    taskManager.addTask("Task 1");
    taskManager.addTask("Task 2");
    taskManager.addTask("Task 3");
    
    const auto& tasks = taskManager.getAllTasks();
    ASSERT_EQ(3, tasks.size());
    
    // Ověříme, že všechna ID jsou jedinečná
    EXPECT_NE(tasks[0].id_, tasks[1].id_);
    EXPECT_NE(tasks[0].id_, tasks[2].id_);
    EXPECT_NE(tasks[1].id_, tasks[2].id_);
}