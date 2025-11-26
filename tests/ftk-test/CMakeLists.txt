set(HEADERS ftk-test.h)

set(SOURCE ftk-test.cpp)

set(LIBRARIES ftkCoreTest ftkUITest)
if ("${ftk_API}" STREQUAL "GL_4_1" OR
    "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
    "${ftk_API}" STREQUAL "GLES_2")
    list(APPEND LIBRARIES ftkGLTest)
endif()

add_executable(ftk-test ${SOURCE} ${HEADERS})
target_link_libraries(ftk-test ${LIBRARIES})
set_target_properties(ftk-test PROPERTIES FOLDER tests)

add_test(
    NAME ftk-test
    COMMAND ftk-test -log)
