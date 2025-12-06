//CMakelists updated “supports the pistache version” and supports c make “connectes cmake with pistache”:
cmake_minimum_required(VERSION 3.16)
project(SWE_lms)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Pistache + RapidJSON via pkg-config ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(PISTACHE REQUIRED libpistache)

include_directories(${PISTACHE_INCLUDE_DIRS})
link_directories(${PISTACHE_LIBRARY_DIRS})
add_definitions(${PISTACHE_CFLAGS_OTHER})

add_executable(SWE_lms
    main.cpp
    book.cpp
    StudyRoom.cpp
    common.cpp
    inventory.cpp
    borrow.cpp
    preferences.cpp
    laptop.cpp
    librarian.cpp
    member.cpp
    auth.cpp
    bookloader.cpp
    export_to_excel.cpp
    server.cpp
)

# --- Link pistache correctly ---
target_link_libraries(SWE_lms ${PISTACHE_LIBRARIES})

# --- Ensure correct architecture ---
set_target_properties(SWE_lms PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
)