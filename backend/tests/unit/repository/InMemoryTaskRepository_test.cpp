#include <gtest/gtest.h>
#include "repository/memory/InMemoryTaskRepository.h"
#include "model/Task.h"

class InMemoryTaskRepositoryTest : public ::testing::Test {
protected:
    InMemoryTaskRepository repository;
};

TEST_F(InMemoryTaskRepositoryTest, AddTask_ShouldStoreTask) {
    Task task{1, "Test Task", false};
    repository.addTask(task.title_);

    auto tasks = repository.getAllTasks();
    ASSERT_EQ(1, tasks.size());
    EXPECT_EQ("Test Task", tasks[0].title_);
    EXPECT_FALSE(tasks[0].done_);
}

TEST_F(InMemoryTaskRepositoryTest, GetAll_ShouldReturnAllTasks) {
    repository.addTask("Task 1");
    repository.addTask("Task 2");

    auto tasks = repository.getAllTasks();
    ASSERT_EQ(2, tasks.size());
}

TEST_F(InMemoryTaskRepositoryTest, Remove_ShouldRemoveTask) {
    auto task = repository.addTask("Test Task");

    bool result = repository.removeTaskById(task.id_);
    EXPECT_TRUE(result);
    EXPECT_TRUE(repository.getAllTasks().empty());
}

TEST_F(InMemoryTaskRepositoryTest, MarkDone_ShouldUpdateTaskStatus) {
    auto task = repository.addTask("Test Task");

    bool result = repository.markDoneById(task.id_);
    EXPECT_TRUE(result);

    auto tasks = repository.getAllTasks();
    ASSERT_FALSE(tasks.empty());
    EXPECT_TRUE(tasks[0].done_);
}

TEST_F(InMemoryTaskRepositoryTest, GetById_ShouldReturnCorrectTask) {
    auto task = repository.addTask("Test Task");

    auto retrievedTask = repository.getTaskById(task.id_);
    ASSERT_TRUE(retrievedTask.has_value());
    EXPECT_EQ("Test Task", retrievedTask->title_);
}