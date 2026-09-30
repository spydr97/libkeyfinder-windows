cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)   # enable CMAKE_MSVC_RUNTIME_LIBRARY selection
project(kfcli CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Match the vcpkg x64-windows-static triplet (static CRT) so linking succeeds.
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(KeyFinder CONFIG REQUIRED)

add_executable(kfcli src/kfcli.cpp)
target_link_libraries(kfcli PRIVATE KeyFinder::keyfinder)
