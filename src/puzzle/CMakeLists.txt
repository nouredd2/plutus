file (GLOB SOURCES "./*.cc")
#add_library (libpuzzle SHARED puzzle.cc crypto_util.cc factory.cc)
add_library (libpuzzle SHARED ${SOURCES})
set_target_properties (libpuzzle PROPERTIES OUTPUT_NAME libpuzzle${BUILD_POSTIFIX})
