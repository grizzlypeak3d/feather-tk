pybind11_add_module(ftkPy Bindings.cpp)

target_include_directories(ftkPy PRIVATE ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(ftkPy PRIVATE ftkUIPy ftkCorePy)

set_target_properties(ftkPy PROPERTIES FOLDER lib)
set_target_properties(ftkPy PROPERTIES PUBLIC_HEADER "${HEADERS}")

# The rpath convention of the executables names the interpreter's
# directory here, since the "executable" loading a Python module is
# Python itself: the module finds the libraries beside it instead.
if(APPLE)
    set_target_properties(ftkPy PROPERTIES INSTALL_RPATH "@loader_path")
elseif(UNIX)
    set_target_properties(ftkPy PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()


install(TARGETS ftkPy
    ARCHIVE DESTINATION lib COMPONENT dev
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
