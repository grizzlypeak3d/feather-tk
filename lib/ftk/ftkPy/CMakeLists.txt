pybind11_add_module(ftkPy Bindings.cpp)
target_link_libraries(ftkPy PRIVATE ftkUIPy ftkCorePy)
set_target_properties(ftkPy PROPERTIES FOLDER lib)
set_target_properties(ftkPy PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS ftkPy
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
