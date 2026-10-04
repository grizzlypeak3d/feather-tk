add_executable(ftk-gpu-test ftk-gpu-test.cpp)
target_include_directories(ftk-gpu-test PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(ftk-gpu-test ftkGPU ftkGL ftkCore)
set_target_properties(ftk-gpu-test PROPERTIES FOLDER tests)
