cmake_minimum_required (VERSION 2.6)
project (Plutus)

# The version number 
set (Plutus_VERSION_MAJOR 0)
set (Plutus_VERSION_MINOR 1)

# find the .cc files
#file (GLOB SOURCES "./*.cc")


#add_executable (client_test.exec ${SOURCES})
#target_link_libraries (client_test.exec m ssl crypto)

include_directories(plutus)
include_directories(./include)
add_subdirectory(src)
