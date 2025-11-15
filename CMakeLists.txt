cmake_minimum_required(VERSION 3.29)
project(SWE_lms)

set(CMAKE_CXX_STANDARD 20)

add_executable(SWE_lms main.cpp
        book.cpp
        book.h
        StudyRoom.cpp
        StudyRoom.h
        common.cpp
        common.h
        inventory.cpp
        inventory.h
        borrow.cpp
        borrow.h
        preferences.cpp
        preferences.h
        laptop.cpp
        laptop.h
        librarian.cpp
        librarian.h
        member.cpp
        member.h
        auth.cpp
        auth.h
        bookloader.cpp
        bookloader.h
        export_to_excel.cpp)
