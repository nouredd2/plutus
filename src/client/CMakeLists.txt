file (GLOB SOURCES "./*.cc")
add_library (libclient SHARED ${SOURCES})
set_target_properties (libclient PROPERTIES OUTPUT_NAME libclient${BUILD_POSTIFIX})
