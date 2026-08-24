cmake_minimum_required(VERSION 3.10)
project(cpp_service)

set(CMAKE_CXX_STANDARD 17)
add_executable(server main.cpp)

# Download cpp-httplib header at build time
include(FetchContent)
FetchContent_Declare(httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG        v0.14.1
)
FetchContent_MakeAvailable(httplib)
target_include_directories(server PRIVATE ${httplib_SOURCE_DIR})
