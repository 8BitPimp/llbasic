#
# .____    .____   __________    _____    _________.____________  
# |    |   |    |  \______   \  /  _  \  /   _____/|   \_   ___ \ 
# |    |   |    |   |    |  _/ /  /_\  \ \_____  \ |   /    \  \/ 
# |    |___|    |___|    |   \/    |    \/        \|   \     \____
# |_______ \_______ \______  /\____|__  /_______  /|___|\______  /
#         \/       \/      \/         \/        \/             \/ 
#   By Aidan Dodds
# 
# ./llbasic/

cmake_minimum_required(VERSION 3.1)
project(llbasic)

set(FLAGS)

if (${UNIX})
    MESSAGE(STATUS "unix build")
    append(FLAGS "-std=c++11")
endif()

if (${MSVC})
    MESSAGE(STATUS "visual studio build")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")

add_subdirectory(llb_frontend)
add_subdirectory(llb_backend_llvm)
add_subdirectory(llb_backend_cpp)
add_subdirectory(llb_passes)
add_subdirectory(llb_tests)
add_subdirectory(llb_driver)
