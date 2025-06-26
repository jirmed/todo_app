#pragma once
#include <string>
#include <cstddef> // Pro std::size_t

struct Task {
    std::size_t id_ = 0;
    std::string title_;
    bool done_ = false;

    Task() = default;

    explicit Task(std::string title, bool done = false)
        : title_(std::move(title)), done_(done) {
    }

};