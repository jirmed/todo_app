#pragma once
#include <string>


struct Task {
    int id_ = -1;
    std::string title_;
    bool done_ = false;

    Task() = default;

    explicit Task(std::string title, bool done = false)
        : title_(std::move(title)), done_(done) {
    }

};
