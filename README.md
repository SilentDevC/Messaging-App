# Messaging-App 👋

![Built With C++](https://img.shields.io/badge/Built%20With-C%2B%2B-blueviolet?style=flat-square&logo=c%2B%2B) ![Boost Libraries](https://img.shields.io/badge/Dependencies-Boost%20Libraries-orange?style=flat-square) ![Vcpkg](https://img.shields.io/badge/Package%20Manager-Vcpkg-success?style=flat-square&logo=vcpkg) ![Status](https://img.shields.io/badge/Status-Under%20Development-lightgrey?style=flat-square)

A messaging application project leveraging the power of C++ Boost Libraries, including **Boost.Beast** for robust networking, **Boost.Asio** for efficient asynchronous I/O, and **Boost.MySQL** for database interactions. This project aims to provide a reliable server-side backend capable of handling user authentication and management, with a client-side application UI planned using the **Dear ImGui Library**. Currently, all features are under active development, focusing on the server-side infrastructure for user operations and data persistence.

## 📝 Table of Contents

*   [🌟 Features](#-features)
*   [🛠️ Tech Stack](#%EF%B8%8F-tech-stack)
*   [🚀 Installation](#-installation)
    *   [Prerequisites](#prerequisites)
    *   [Dependencies](#dependencies)
    *   [Building the Server](#building-the-server)
*   [💡 Usage](#-usage)
    *   [Running the Server](#running-the-server)
    *   [API Endpoints](#api-endpoints)
*   [📁 Project Structure](#-project-structure)
*   [📖 API Reference](#-api-reference)
*   [🤝 Contributing](#-contributing)
*   [⚖️ License](#%EF%B8%8F-license)
*   [🔗 Important Links](#-important-links)
*   [🙏 Footer](#-footer)

## 🌟 Features

This project, though still in development, demonstrates core functionalities for a robust messaging backend:

*   **Asynchronous Network Communication** ⚡: Utilizes Boost.Asio for non-blocking I/O operations, ensuring efficient handling of multiple client connections.
*   **HTTP Server Implementation** 🌐: Built with Boost.Beast to serve HTTP requests, providing a foundation for RESTful API interactions.
*   **User Management API** 👤: Supports basic CRUD (Create, Update, Delete) operations for user accounts via dedicated HTTP endpoints.
    *   **User Creation**: Register new users with unique usernames, emails, and password hashes.
    *   **User Update**: Modify existing user details suchs as email or password hash.
    *   **User Deletion**: Remove user accounts from the system.
*   **MySQL Database Integration** 🗄️: Persistent storage for user data using Boost.MySQL, ensuring data integrity and reliability.
*   **Database Worker Pool** ⚙️: Asynchronous processing of database queries through a dedicated worker thread, preventing blocking of the main server loop.
*   **Request Data Parsing** ➡️: Handles incoming JSON payloads for user data, converting them into internal data structures.
*   **Detailed Logging** 🪵: Includes mechanisms for logging server activities and errors.

## 🛠️ Tech Stack

The Messaging-App server is built using the following technologies:

*   **Primary Language**: C++
*   **Networking**: 
    *   [Boost.Asio](https://www.boost.org/doc/libs/release/libs/asio/)
    *   [Boost.Beast](https://www.boost.org/doc/libs/release/libs/beast/)
*   **Database**: 
    *   [Boost.MySQL](https://www.boost.org/doc/libs/release/libs/mysql/)
    *   MySQL Server
*   **JSON Handling**: 
    *   [Boost.JSON](https://www.boost.org/doc/libs/release/libs/json/)
    *   [nlohmann/json](https://github.com/nlohmann/json)
*   **Utilities**: 
    *   [Boost.Date_Time](https://www.boost.org/doc/libs/release/libs/date_time/)
*   **Package Management**: [Vcpkg](https://vcpkg.io/)
*   **Client UI (Planned)**: [Dear ImGui](https://github.com/ocornut/imgui)

## 🚀 Installation

To get the Messaging-App server up and running on your local machine, follow these steps.

### Prerequisites

Before you begin, ensure you have the following installed:

*   **C++ Compiler**: A C++17 compatible compiler (e.g., MSVC, GCC, Clang).
*   **Vcpkg** (recommended package manager): Follow the official [Vcpkg installation guide](https://vcpkg.io/en/docs/getting-started/installing.html).
*   **MySQL Server**: A running MySQL server instance. The application is configured to connect to `localhost:3306` with username `root` and password `admin` by default. You might need to adjust these credentials or set up a local MySQL instance accordingly.

### Dependencies

This project uses Vcpkg to manage its C++ dependencies. The `vcpkg-configuration.json` specifies the default registry.

After installing Vcpkg, navigate to the project root directory and install the necessary libraries:

```bash
vcpkg install
```

This command will install Boost.Asio, Boost.Beast, Boost.MySQL, nlohmann/json, and other required Boost modules.

### Building the Server

The project includes solution files for Visual Studio (`.sln`, `.vcxproj`) and a `CMakeLists.txt`, indicating support for both build systems.

**Option 1: Using Visual Studio (Windows)**

1.  Open `ChatServer-Application.sln` with Visual Studio.
2.  Build the `ChatServer-Application` project.

**Option 2: Using CMake (Cross-platform)**

1.  Create a build directory:
    ```bash
mkdir build
cd build
    ```
2.  Configure CMake. If using Vcpkg, you might need to specify the toolchain file:
    ```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake
    ```
    (Replace `[path_to_vcpkg]` with your actual Vcpkg installation path).
3.  Build the project:
    ```bash
cmake --build .
    ```

Upon successful compilation, an executable named `ChatServer-Application` (or `ChatServer-Application.exe` on Windows) will be generated.

## 💡 Usage

### Running the Server

After building the project, you can run the server application. The server is configured to listen for incoming connections on `127.0.0.1:8080`.

```bash
./path/to/your/build/ChatServer-Application
```

You will see output indicating that the server has started and is listening on the specified address and port.

### API Endpoints

The server exposes an API for user management. Below are examples of how to interact with the server using `cURL`.

**Base URL**: `http://127.0.0.1:8080`

#### ➡️ Create User (`POST /users`)

To create a new user, send a POST request to the `/users` endpoint with a JSON body containing the user's details. The `created_at` field should be a date string (e.g., "2023-01-01").

```bash
curl -X POST \
     -H "Content-Type: application/json" \
     -d '{"username": "newuser", "email": "newuser@example.com", "phash": "securepasswordhash", "created_at": "2023-10-26", "is_active": true}' \
     http://127.0.0.1:8080/users
```

#### 🔄 Update User (`PUT /users`)

To update an existing user's information, send a PUT request to the `/users` endpoint. The `username` field is used to identify the user.

```bash
curl -X PUT \
     -H "Content-Type: application/json" \
     -d '{"username": "newuser", "email": "updated_email@example.com", "phash": "newsecurehash", "created_at": "2023-10-26", "is_active": true}' \
     http://127.0.0.1:8080/users
```

#### 🗑️ Delete User (`DELETE /users`)

To delete a user account, send a DELETE request to the `/users` endpoint with the username in the JSON body.

```bash
curl -X DELETE \
     -H "Content-Type: application/json" \
     -d '{"username": "newuser"}' \
     http://127.0.0.1:8080/users
```

## 📁 Project Structure

The project follows a clear structure for better organization:

```
. # Project Root
├── ChatServer-Application.cpp # Main server entry point
├── ChatServer-Application.sln # Visual Studio Solution file
├── ChatServer-Application.vcxproj # Visual Studio Project file
├── CMakeLists.txt # CMake build configuration
├── Header/ # Contains header files for various modules
│   ├── Client.hpp # Client-side definitions (planned/in-dev)
│   ├── Concurency.hpp # Concurrency utilities
│   ├── DataStructures.hpp # Common data structures (e.g., user_data)
│   ├── Logger.hpp # Logging functionalities
│   ├── MYSQL_DB_connect.hpp # MySQL database connection interface
│   ├── Server.hpp # Server class definition
│   ├── Types.hpp # Custom type definitions
│   └── Utils.hpp # General utility functions
├── Source/ # Contains implementation files
│   ├── MySQL_DB_Connect.cpp # MySQL database connection implementation
│   ├── Route_functions.cpp # HTTP route handling functions
│   ├── Server.cpp # Server logic implementation
│   └── Utils.cpp # Utility function implementations
├── packages.config # NuGet package configuration (if applicable)
├── README.md # This README file
├── vcpkg-configuration.json # Vcpkg configuration for baseline and registries
└── vcpkg.json # Vcpkg manifest for project dependencies
```

## 📖 API Reference

The Messaging-App server provides the following primary API endpoint for managing user accounts:

### `/users`

*   **Method**: `POST`
    *   **Description**: Creates a new user account.
    *   **Request Body (`application/json`)**: 
        ```json
        {
          "username": "string",
          "email": "string",
          "phash": "string",
          "created_at": "YYYY-MM-DD",
          "is_active": boolean
        }
        ```
    *   **Response**: Indicates success or failure of user creation.

*   **Method**: `PUT`
    *   **Description**: Updates an existing user account's details.
    *   **Request Body (`application/json`)**: 
        ```json
        {
          "username": "string",
          "email": "string",
          "phash": "string",
          "created_at": "YYYY-MM-DD",
          "is_active": boolean
        }
        ```
        _The `username` field is required for identification._
    *   **Response**: Indicates success or failure of user update.

*   **Method**: `DELETE`
    *   **Description**: Deletes a user account.
    *   **Request Body (`application/json`)**: 
        ```json
        {
          "username": "string"
        }
        ```
    *   **Response**: Indicates success or failure of user deletion.

## 🤝 Contributing

Contributions are welcome! If you'd like to contribute, please follow these steps:

1.  Fork the repository.
2.  Create a new branch for your feature or bug fix.
3.  Make your changes and ensure the code adheres to the existing style.
4.  Write clear, concise commit messages.
5.  Submit a pull request.

## ⚖️ License

This project is under **MIT License**.

## 🔗 Important Links

*   No important links (e.g., live demo, user profiles) are available at this time.

## 🙏 Footer

--- 

Made with ❤️ by SilentDevC

Project: [Messaging-App](https://github.com/SilentDevC/Messaging-App)

Feel free to ⭐ star the repository, fork it, and open issues for any bugs or feature requests! Your support is highly appreciated! 😊
