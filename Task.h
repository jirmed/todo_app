#pragma once
#include <string>


struct Task {
    std::string title_;
    bool done_ = false;

    Task() = default;

    explicit Task(std::string &title, bool done = false)
        : title_(std::move(title)),
          done_(done) {
    }
};
