add_executable(simple simple.cpp)

target_include_directories(simple PRIVATE ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR})

target_link_libraries(simple ftkUI)

set_target_properties(simple PROPERTIES FOLDER examples)

add_test(NAME simple COMMAND simple -exit -log)
