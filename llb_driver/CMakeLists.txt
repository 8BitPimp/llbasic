#
# .____    .____   __________    _____    _________.____________  
# |    |   |    |  \______   \  /  _  \  /   _____/|   \_   ___ \ 
# |    |   |    |   |    |  _/ /  /_\  \ \_____  \ |   /    \  \/ 
# |    |___|    |___|    |   \/    |    \/        \|   \     \____
# |_______ \_______ \______  /\____|__  /_______  /|___|\______  /
#         \/       \/      \/         \/        \/             \/ 
#   By Aidan Dodds
# 
# ./llbasic/llb_driver/

file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)
add_executable(llb_driver ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(llb_driver LINK_PUBLIC llb_frontend llb_passes llb_backend_cpp llb_util)
