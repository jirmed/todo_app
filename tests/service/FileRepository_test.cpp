#include <gtest/gtest.h>
#include "../../src/repository/FileRepository.h"
#include <filesystem>
#include <fstream>

class FileRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        cleanupTestFile();
    }

    void TearDown() override {
        cleanupTestFile();
    }

    void cleanupTestFile() {
        if (std::filesystem::exists(testFile)) {
            std::filesystem::remove(testFile);
        }
    }

    const std::string testFile = "test_tasks.csv";
    FileRepository repository{testFile};
};

TEST_F(FileRepositoryTest, SaveTask_ShouldPersistTask) {
    Task task = repository.addTask("Test Task");

    std::ifstream file(testFile);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    EXPECT_TRUE(content.find("Test Task") != std::string::npos);
}

TEST_F(FileRepositoryTest, GetAllTasks_ShouldReturnSavedTasks) {
    Task task1 = repository.addTask("Task 1");
    Task task2 = repository.addTask("Task 2");
    repository.markDoneById(task2.id_);

    auto tasks = repository.getAllTasks();
    EXPECT_EQ(2, tasks.size());
    EXPECT_EQ("Task 1", tasks[0].title_);
    EXPECT_EQ("Task 2", tasks[1].title_);
    EXPECT_FALSE(tasks[0].done_);
    EXPECT_TRUE(tasks[1].done_);
}

TEST_F(FileRepositoryTest, GetTaskById_ShouldReturnCorrectTask) {
    Task task = repository.addTask("Test Task");

    auto foundTask = repository.getTaskById(task.id_);
    EXPECT_TRUE(foundTask.has_value());
    EXPECT_EQ("Test Task", foundTask->title_);
}

TEST_F(FileRepositoryTest, RemoveTask_ShouldDeleteTask) {
    Task task = repository.addTask("Test Task");

    bool removed = repository.removeTaskById(task.id_);
    EXPECT_TRUE(removed);

    auto tasks = repository.getAllTasks();
    EXPECT_TRUE(tasks.empty());
}

TEST_F(FileRepositoryTest, GetTaskById_NonExistentTask_ShouldReturnNullopt) {
    auto task = repository.getTaskById(999);
    EXPECT_FALSE(task.has_value());
}