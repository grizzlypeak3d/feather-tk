# The module is private to the feather_tk package, whose __init__.py gathers
# its API; the build tree lays the package out the way an install does, so
# PYTHONPATH can name either.
set(ftk_PYTHON_PACKAGE_DIR ${CMAKE_BINARY_DIR}/python/feather_tk)

set(STABLE_ABI)
if(ftk_PYTHON_STABLE_ABI)
    set(STABLE_ABI STABLE_ABI)
endif()
nanobind_add_module(_feather_tk ${STABLE_ABI} NB_STATIC Bindings.cpp)

target_include_directories(_feather_tk PRIVATE ${PROJECT_SOURCE_DIR}/lib)

# The binding objects go into the module directly rather than through
# their archives. nanobind's own static library is first on the link line,
# and GNU ld takes from an archive only what is wanted at the moment it is
# scanned: a binding archive scanned after it finds the enum and datetime
# helpers already passed over, and a Release build fails to link.
target_sources(_feather_tk PRIVATE $<TARGET_OBJECTS:ftkUIPy> $<TARGET_OBJECTS:ftkCorePy>)
target_link_libraries(_feather_tk PRIVATE ftkUIPy ftkCorePy)

# The generator expression keeps multi-config generators from adding a
# directory per configuration, which would leave the module outside the
# package.
set_target_properties(_feather_tk PROPERTIES
    FOLDER lib
    LIBRARY_OUTPUT_DIRECTORY $<1:${ftk_PYTHON_PACKAGE_DIR}>)
configure_file(__init__.py ${ftk_PYTHON_PACKAGE_DIR}/__init__.py COPYONLY)

# The rpath convention of the executables names the interpreter's
# directory here, since the "executable" loading a Python module is
# Python itself: the module finds the libraries from where it is instead.
# That is <prefix>/lib from <prefix>/lib/feather_tk, and lib inside the
# package in a wheel.
file(RELATIVE_PATH ftk_PYTHON_LIB_RPATH
    /prefix/${ftk_PYTHON_INSTALL_DIR} /prefix/lib)
if(APPLE)
    set_target_properties(_feather_tk PROPERTIES
        INSTALL_RPATH "@loader_path/${ftk_PYTHON_LIB_RPATH}")
elseif(UNIX)
    set_target_properties(_feather_tk PROPERTIES
        INSTALL_RPATH "$ORIGIN/${ftk_PYTHON_LIB_RPATH}")
endif()

install(TARGETS _feather_tk
    LIBRARY DESTINATION ${ftk_PYTHON_INSTALL_DIR}
    RUNTIME DESTINATION ${ftk_PYTHON_INSTALL_DIR})
install(
    FILES __init__.py
    DESTINATION ${ftk_PYTHON_INSTALL_DIR})
