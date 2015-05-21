#
# .____    .____   __________    _____    _________.____________  
# |    |   |    |  \______   \  /  _  \  /   _____/|   \_   ___ \ 
# |    |   |    |   |    |  _/ /  /_\  \ \_____  \ |   /    \  \/ 
# |    |___|    |___|    |   \/    |    \/        \|   \     \____
# |_______ \_______ \______  /\____|__  /_______  /|___|\______  /
#         \/       \/      \/         \/        \/             \/ 
#   By Aidan Dodds
# 
# ./llbasic/llb_runtime/

file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

add_library(llb_runtime ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(llb_runtime LINK_PUBLIC llb_util)
