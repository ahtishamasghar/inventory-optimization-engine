
cmake_minimum_required(VERSION 3.16)

project(InventoryOptimization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(inventory_server main.cpp)

target_include_directories(inventory_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/crow/include
)

target_link_libraries(inventory_server PRIVATE Threads::Threads)
