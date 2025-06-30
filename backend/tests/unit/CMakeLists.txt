# CMakeLists.txt pro testy
# Testy pro projekt todo_app
include(GoogleTest)

# Najdeme všechny testovací soubory
file(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/**/*_test.cpp")

# Výpis nalezených testovacích souborů
message(STATUS "Nalezené testovací soubory:")
foreach(TEST_FILE ${TEST_FILES})
    message(STATUS "  ${TEST_FILE}")
endforeach()

# Vytvořte samostatné test executables pro každý testovací soubor
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})

    target_include_directories(${TEST_NAME} PRIVATE ..)
    target_link_libraries(${TEST_NAME} PRIVATE core gtest_main gtest)

    # Windows output adresář
    if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(${TEST_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    endif()

    # Automatické objevení testů
    gtest_discover_tests(${TEST_NAME}
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_NAME}>"
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_NAME}>"
        TEST_PREFIX "${TEST_NAME}."
        TEST_LIST ${TEST_NAME}_TESTS)

    set_property(GLOBAL PROPERTY TEST_DISCOVERY_TIMEOUT 60)
endforeach()

# Jednotný executable pro všechny testy
add_executable(unit_tests ${TEST_FILES})
target_include_directories(unit_tests PRIVATE ..)
target_link_libraries(unit_tests PRIVATE core gtest_main gtest)

# Windows output adresář
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(unit_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()
include(GoogleTest)

# Najděte všechny testovací soubory
file(GLOB_RECURSE TEST_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*_test.cpp"
)

# Vypíše seznam nalezených testovacích souborů
message(STATUS "Nalezené testovací soubory:")
foreach(TEST_FILE ${TEST_FILES})
    message(STATUS "  ${TEST_FILE}")
endforeach()

# Vytvořte samostatné test executables pro každý testovací soubor
foreach(TEST_FILE ${TEST_FILES})
    # Získat název testu bez cesty a přípony
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # Přidat samostatný test executable
    add_executable(${TEST_NAME} ${TEST_FILE})

    target_include_directories(${TEST_NAME}
        PRIVATE
            ..
    )

    target_link_libraries(${TEST_NAME}
        PRIVATE
        core
        gtest_main
        gtest
    )

    # Windows output adresář pro testy
    if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(${TEST_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )
    endif()

    # Použití gtest_discover_tests místo add_test pro detekci jednotlivých testovacích případů
    gtest_discover_tests(${TEST_NAME}
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_NAME}>"
        PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_NAME}>"
        TEST_PREFIX "${TEST_NAME}."
        TEST_LIST ${TEST_NAME}_TESTS
    )

    # Nastavení vlastnosti TEST_DISCOVERY_TIMEOUT pro delší dobu objevování testů
    set_property(GLOBAL PROPERTY TEST_DISCOVERY_TIMEOUT 60)
endforeach()

# Ponecháme i původní unit_tests executable pro zpětnou kompatibilitu
add_executable(unit_tests ${TEST_FILES})

target_include_directories(unit_tests
    PRIVATE
        ..
)

target_link_libraries(unit_tests
    PRIVATE
    core
    gtest_main
    gtest
)

# Windows output adresář pro testy
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(unit_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endif()
