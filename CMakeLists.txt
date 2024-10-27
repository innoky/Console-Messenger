cmake_minimum_required(VERSION 3.10) # Минимальная версия CMake
project(MyProject)                   # Название проекта

set(CMAKE_CXX_STANDARD 11)          # Установка стандарта C++
set(Boost_USE_STATIC_LIBS OFF)

find_package(Boost REQUIRED COMPONENTS system)

       

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(chat_client src/chat_client.cpp)
    add_executable(chat_server src/chat_server.cpp)

    # Связать библиотеки Boost и pthread
    target_link_libraries(chat_server ${Boost_LIBRARIES} pthread)
    target_link_libraries(chat_client ${Boost_LIBRARIES} pthread)
else()
    message(FATAL_ERROR "Boost not found.")
endif()
