#pragma once
#include <string_view>

namespace messages {
    // Success messages
    inline constexpr std::string_view TASK_ADDED = "Úkol byl přidán!";
    inline constexpr std::string_view TASK_REMOVED = "Úkol byl odstraněn!";
    inline constexpr std::string_view TASK_COMPLETED = "Úkol byl označen jako dokončený!";

    // Error messages
    inline constexpr std::string_view INVALID_TASK_NUMBER = "Neplatné číslo úkolu!";
    inline constexpr std::string_view INVALID_CHOICE = "Neplatná volba! Zkuste to znovu.";

    // Info messages
    inline constexpr std::string_view NO_TASKS = "Žádné úkoly.";
    inline constexpr std::string_view APP_TITLE = "=== TODO APLIKACE ===";
    inline constexpr std::string_view MENU_TITLE = "\n--- MENU ---";
    inline constexpr std::string_view EXITING = "Ukončuji aplikaci...";
    inline constexpr std::string_view TASKS_LIST_TITLE = "\nSeznam úkolů:";

    // Prompts
    inline constexpr std::string_view MENU_PROMPT = "Vyberte možnost (1-5): ";
    inline constexpr std::string_view TASK_TITLE_PROMPT = "Zadejte název úkolu: ";
    inline constexpr std::string_view TASK_INDEX_PROMPT = "Zadejte číslo úkolu: ";

    // Menu options
    inline constexpr std::string_view MENU_SHOW_TASKS = "1. Zobrazit úkoly";
    inline constexpr std::string_view MENU_ADD_TASK = "2. Přidat úkol";
    inline constexpr std::string_view MENU_REMOVE_TASK = "3. Odstranit úkol";
    inline constexpr std::string_view MENU_MARK_DONE = "4. Označit úkol jako dokončený";
    inline constexpr std::string_view MENU_EXIT = "5. Ukončit";
}
