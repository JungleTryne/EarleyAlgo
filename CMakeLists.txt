cmake_minimum_required(VERSION 3.17)

project(Earley)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wa,-mbig-obj")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -Wa,-mbig-obj")

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(Earley
        main.cpp
        src/Earley.cpp
        src/Grammar.cpp
        src/GrammarFetchers.cpp
        )

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

add_executable(EarleyTest

        tests/test.cpp
        tests/test-earley.cpp
        tests/test-fetcher.cpp

        src/Earley.cpp
        src/Grammar.cpp
        src/GrammarFetchers.cpp
        )

target_link_libraries(EarleyTest Catch2::Catch2)

install(TARGETS Earley EarleyTest DESTINATION ${CMAKE_INSTALL_PREFIX}/debug CONFIGURATIONS DEBUG)
install(TARGETS Earley EarleyTest DESTINATION ${CMAKE_INSTALL_PREFIX}/release CONFIGURATIONS RELEASE)

