nanobind_add_module(ftkPy NB_STATIC Bindings.cpp)

target_include_directories(ftkPy PRIVATE ${PROJECT_SOURCE_DIR}/lib)

# The binding objects go into the module directly rather than through
# their archives. nanobind's own static library is first on the link line,
# and GNU ld takes from an archive only what is wanted at the moment it is
# scanned: a binding archive scanned after it finds the enum and datetime
# helpers already passed over, and a Release build fails to link.
target_sources(ftkPy PRIVATE $<TARGET_OBJECTS:ftkUIPy> $<TARGET_OBJECTS:ftkCorePy>)
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
