add_executable(ftk-test ftk-test.h ftk-test.cpp)

target_include_directories(ftk-test PRIVATE ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(ftk-test ftkUITest)
if ("${ftk_API}" STREQUAL "GL_4_1" OR
    "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
    "${ftk_API}" STREQUAL "GLES_2")
    target_link_libraries(ftk-test ftkGLTest)
endif()
target_link_libraries(ftk-test ftkCoreTest)

set_target_properties(ftk-test PROPERTIES FOLDER tests)

# The macOS and Windows CI runners have no working OpenGL. Leaving out the
# tests that make a context is better than skipping the suite, which is what
# those platforms did before.
if(ftk_TESTS_NO_GL)
    add_test(
        NAME ftk-test
        COMMAND ftk-test -log -noGL)
else()
    add_test(
        NAME ftk-test
        COMMAND ftk-test -log)
endif()

# A test that hangs should fail, not run until the job is killed: this one
# waits on futures, and a reader that never answers waits forever. The whole
# suite is a couple of minutes, so this only fires on a hang.
set_tests_properties(ftk-test PROPERTIES TIMEOUT 900)
