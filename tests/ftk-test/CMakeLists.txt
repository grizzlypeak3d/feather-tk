set(HEADERS ftk-test.h)

set(SOURCE ftk-test.cpp)

add_executable(ftk-test ${SOURCE} ${HEADERS})

target_include_directories(ftk-test PRIVATE ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(ftk-test ftkUITest)
if ("${ftk_API}" STREQUAL "GL_4_1" OR
    "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
    "${ftk_API}" STREQUAL "GLES_2")
    target_link_libraries(ftk-test ftkGLTest)
endif()
target_link_libraries(ftk-test ftkCoreTest)

set_target_properties(ftk-test PROPERTIES FOLDER tests)

add_test(
    NAME ftk-test
    COMMAND ftk-test -log)
