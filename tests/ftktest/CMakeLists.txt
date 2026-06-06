set(HEADERS ftktest.h)

set(SOURCE ftktest.cpp)

add_executable(ftktest ${SOURCE} ${HEADERS})

target_include_directories(ftktest PRIVATE ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(ftktest ftkUITest)
if ("${ftk_API}" STREQUAL "GL_4_1" OR
    "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
    "${ftk_API}" STREQUAL "GLES_2")
    target_link_libraries(ftktest ftkGLTest)
endif()
target_link_libraries(ftktest ftkCoreTest)

set_target_properties(ftktest PROPERTIES FOLDER tests)

add_test(
    NAME ftktest
    COMMAND ftktest -log)
