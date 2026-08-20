add_executable(windows windows.cpp)

target_include_directories(windows PRIVATE ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR})

target_link_libraries(windows ftkUI)

set_target_properties(windows PROPERTIES FOLDER examples)

# The example opens a window, so it cannot run where there is no
# OpenGL. It is still built, which is the compile check.
if(NOT ftk_TESTS_NO_GL)
    add_test(NAME windows COMMAND windows -exit -log)
endif()
