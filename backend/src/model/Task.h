#pragma once

#include <string>
#include <cstddef>
#include <utility> // Pro std::move

struct Task {
    std::size_t id_;
    std::string title_;
    bool done_;

    // Konstruktor pro vytvoření nového úkolu s automaticky generovaným ID (0 je placeholder)
    explicit Task(std::string title) : id_(0), title_(std::move(title)), done_(false) {}

    // Konstruktor pro deserializaci nebo ruční nastavení ID
    Task(std::size_t id, std::string title, bool done) : id_(id), title_(std::move(title)), done_(done) {}
};