add_executable(windows windows.cpp)

target_include_directories(windows PRIVATE ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR})

target_link_libraries(windows ftkUI)

set_target_properties(windows PROPERTIES FOLDER examples)

add_test(NAME windows COMMAND windows -exit -log)
