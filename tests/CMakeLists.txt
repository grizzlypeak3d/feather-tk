add_subdirectory(CoreTest)
add_subdirectory(TestLib)
add_subdirectory(UITest)
add_subdirectory(ftk-test)

if ("${ftk_API}" STREQUAL "GL_4_1" OR
    "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
    "${ftk_API}" STREQUAL "GLES_2")
    add_subdirectory(GLTest)
endif()

if(ftk_PYTHON)
    add_test(
        NAME Python
        COMMAND ${Python3_EXECUTABLE} -m unittest discover -v ${CMAKE_CURRENT_SOURCE_DIR})
endif()
