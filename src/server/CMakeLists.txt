file (GLOB SOURCES "./*.cc")
add_library (libserver SHARED ${SOURCES})
set_target_properties (libserver PROPERTIES OUTPUT_NAME libserver${BUILD_POSTIFIX})
