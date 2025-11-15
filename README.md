# LMS
Library Management System

A modular C++ backend paired with a modern web-based frontend.

Overview

The Library Management System (LMS) is a complete digital platform for managing books, laptops, study rooms, members, librarians, borrowing operations, user activity, and administrative tasks.

The backend is fully implemented in C++ and exposes a REST API.
The frontend is built using HTML, CSS, and JavaScript, providing a clean and responsive user interface.

Architecture

The system is built using a two-tier architecture consisting of a frontend client and a backend server.

Frontend (Client Layer)

The frontend is responsible for all user interactions and rendering. It is implemented using:

HTML for page structure

CSS for styling and layout

JavaScript for dynamic behavior and API communication

Key responsibilities include:

Displaying books, laptops, rooms, wishlist, and user activity

Handling user actions such as searching, borrowing, returning, and reserving

Managing authentication and user sessions

Sending requests to the backend using the Fetch API

Rendering received JSON data into UI components

All client-side logic resides in script.js.

Backend (Application Layer)

The backend is implemented entirely in C++ using a modular class-based design.
It includes components that handle:

Books and book details

Laptops and device management

Study room schedules

Members and librarians

Borrowing and returning operations

Authentication and user preferences

Inventory and administrative operations

A dedicated REST API layer (server.cpp) exposes this functionality to the frontend.
Requests from the frontend are received via HTTP, processed using the backend logic, and returned as structured JSON responses.

Communication Between Frontend and Backend

The frontend and backend communicate through:

HTTP GET and POST requests

JSON-formatted request bodies and responses

Examples:

Searching for books triggers a GET request to /api/search?query=...

Borrowing a book sends a POST request to /api/borrow/{id}

Reserving a study room sends a POST request with a JSON payload to /api/rooms/book

The backend processes these requests using the underlying C++ modules and returns JSON data, which the frontend uses to update the interface.

Project Structure
root/
│── server.cpp
│── server.h
│── main.cpp
│── CMakeLists.txt
│
│── auth.cpp / auth.h
│── book.cpp / book.h
│── bookloader.cpp / bookloader.h
│── borrow.cpp / borrow.h
│── common.cpp / common.h
│── inventory.cpp / inventory.h
│── laptop.cpp / laptop.h
│── librarian.cpp / librarian.h
│── member.cpp / member.h
│── preferences.cpp / preferences.h
│── StudyRoom.cpp / StudyRoom.h
│
└── LibraryFrontend/
     ├── index.html
     ├── style.css
     └── script.js

API Overview
Books
GET    /api/books
GET    /api/books/{id}
GET    /api/search?query={text}
POST   /api/borrow/{id}
POST   /api/return/{id}

Laptops
GET    /api/laptops
POST   /api/laptops/{id}/borrow
POST   /api/laptops/{id}/return

Study Rooms
GET    /api/rooms
GET    /api/rooms/slots?date=YYYY-MM-DD
POST   /api/rooms/book


Example request body:

{
  "roomId": 1,
  "date": "2025-01-01",
  "slot": "10:00-11:00"
}

Authentication
POST   /api/login
POST   /api/signup

Frontend Features

Landing page

Book browsing and search

Borrowing and wishlist system

Laptop management

Study room scheduling with available time slots

Activity tracking

Admin overview panels

Login and signup modal

Light and dark theme toggle

Fully dynamic UI using JavaScript

Backend Features

Modular C++ design with separate classes for each subsystem

Complete borrowing and return logic

Room booking mechanisms

Laptop management

Admin and inventory operations

JSON handling for all REST responses

Central REST API server built in C++

Build System

The backend uses:

CMake as the build system

C++17

Pistache for HTTP server functionality

nlohmann/json for JSON parsing

The frontend requires no build steps.
