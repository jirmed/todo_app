#pragma once
#include "crow.h"

struct SimpleCORS {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context&) const {
        // Odpoví na preflight (OPTIONS) požadavek
        if (req.method == "OPTIONS"_method) {
            add_cors_headers(res);
            res.end();
        }
    }
    void after_handle(crow::request&, crow::response& res, context&) const {
        add_cors_headers(res);
    }

    static void add_cors_headers(crow::response& res) {
        res.add_header("Access-Control-Allow-Origin", "http://localhost:5173"); // nebo "*"
        res.add_header("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE,OPTIONS,PATCH");
        res.add_header("Access-Control-Allow-Headers", "Content-Type,Authorization");
        // Pokud potřebuješ cookies, odkomentuj:
        // res.add_header("Access-Control-Allow-Credentials", "true");
    }
};
