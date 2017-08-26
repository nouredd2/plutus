cmake_minimum_required (VERSION 2.6)
project (Plutus)

# The version number 
set (Plutus_VERSION_MAJOR 0)
set (Plutus_VERSION_MINOR 1)

include_directories(plutus)
include_directories(./include)
add_subdirectory(src)
