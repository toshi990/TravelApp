# High-Performance Excursion Management System

A C++20 backend system for managing excursion data with high-speed search capabilities, custom data structures, and parallel computing.

## Key Features

### Custom Hybrid HashTable
A high-performance storage engine featuring a global doubly linked list. Achieves O(1) iteration complexity regardless of the load factor, outperforming standard std::unordered_map for full-container traversal.

### Data Versioning
Built-in support for historical data. Each record maintains a version chain, enabling state tracking and audits.

### Parallel Search Engine
Multi-threaded closest match search using Fork-Join pattern with std::async. Features configurable timeouts and dynamic thread pooling for maximum CPU utilization.

### Compile-time Safety
Heavy use of C++20 Concepts and static_assert to validate iterator traits and type safety during compilation.

### SOLID Architecture
Clean separation of concerns using MVC and Repository patterns. The design follows SOLID principles:

- **Single Responsibility:** Each class has one clearly defined purpose
- **Open/Closed:** System is open for extension but closed for modification
- **Liskov Substitution:** Derived classes maintain base class contracts
- **Interface Segregation:** Specific interfaces instead of general-purpose ones
- **Dependency Inversion:** High-level modules independent of low-level implementations

## Tech Stack

- **Language:** C++20
- **Parallelism:** std::thread, std::future, std::async
- **Standards:** STL, RAII, Smart Pointers (unique_ptr)
- **Build System:** CMake (minimum version 3.15)

## Build Instructions

### Prerequisites
- CMake 3.15 or higher
- C++20 compatible compiler

### Build Steps

```bash
# Clone the repository
git clone <repository-url>
cd TravelApp

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .

# Run the application
./app

# Run tests
cmake --build . --target run_tests