#
# .____    .____   __________    _____    _________.____________  
# |    |   |    |  \______   \  /  _  \  /   _____/|   \_   ___ \ 
# |    |   |    |   |    |  _/ /  /_\  \ \_____  \ |   /    \  \/ 
# |    |___|    |___|    |   \/    |    \/        \|   \     \____
# |_______ \_______ \______  /\____|__  /_______  /|___|\______  /
#         \/       \/      \/         \/        \/             \/ 
#   By Aidan Dodds
# 
# ./llbasic/llb_passes/

file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

add_library(llb_passes ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(llb_passes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
include_directories("../llb_frontend")
