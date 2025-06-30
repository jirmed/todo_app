#include <gtest/gtest.h>
#include "repository/file/FileRepository.h"
#include <filesystem>
#include <fstream>
#include <memory>

class FileRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream ofs(testFile, std::ios::out | std::ios::trunc);
        ofs.close();

        repository = std::make_unique<FileRepository>(testFile);
    }

    void TearDown() override {
        repository.reset();
        if (std::filesystem::exists(testFile)) {
            std::filesystem::remove(testFile);
        }
    }

    const std::string testFile = "test_tasks.csv";
    std::unique_ptr<FileRepository> repository;
};
TEST_F(FileRepositoryTest, FileDoesNotExist_ShouldCreateFile) {
    // Odstranění souboru, aby ho mohl repository vytvořit
    if (std::filesystem::exists(testFile)) {
        std::filesystem::remove(testFile);
    }

    // Vytvoření nové instance repository
    auto newRepo = std::make_unique<FileRepository>(testFile);

    // Ověření, že soubor byl vytvořen
    EXPECT_TRUE(std::filesystem::exists(testFile));

    // Přidání úkolu pro ověření funkčnosti
    newRepo->addTask("Nový úkol");

    // Ověření, že úkol byl úspěšně uložen
    auto tasks = newRepo->getAllTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_EQ("Nový úkol", tasks[0].title_);
}


TEST_F(FileRepositoryTest, SaveTask_ShouldPersistTask) {
    Task task = repository->addTask("Test Task");

    std::ifstream file(testFile);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    EXPECT_TRUE(content.find("Test Task") != std::string::npos);
}

TEST_F(FileRepositoryTest, GetAllTasks_ShouldReturnSavedTasks) {
    Task task1 = repository->addTask("Task 1");
    Task task2 = repository->addTask("Task 2");
    repository->markDoneById(task2.id_);

    auto tasks = repository->getAllTasks();
    EXPECT_EQ(2, tasks.size());
    EXPECT_EQ("Task 1", tasks[0].title_);
    EXPECT_EQ("Task 2", tasks[1].title_);
    EXPECT_FALSE(tasks[0].done_);
    EXPECT_TRUE(tasks[1].done_);
}

TEST_F(FileRepositoryTest, GetTaskById_ShouldReturnCorrectTask) {
    Task task = repository->addTask("Test Task");

    auto foundTask = repository->getTaskById(task.id_);
    EXPECT_TRUE(foundTask.has_value());
    EXPECT_EQ("Test Task", foundTask->title_);
}

TEST_F(FileRepositoryTest, RemoveTask_ShouldDeleteTask) {
    Task task = repository->addTask("Test Task");

    bool removed = repository->removeTaskById(task.id_);
    EXPECT_TRUE(removed);

    auto tasks = repository->getAllTasks();
    EXPECT_TRUE(tasks.empty());
}

TEST_F(FileRepositoryTest, GetTaskById_NonExistentTask_ShouldReturnNullopt) {
    auto task = repository->getTaskById(999);
    EXPECT_FALSE(task.has_value());
}
