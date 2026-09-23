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

# Type stubs: the signatures of a compiled module in a form editors and
# type checkers can read, since there is no Python source in it for them to
# parse. Written by nanobind from the bindings themselves, which imports the
# module that was just built -- through etc/Python/make_stub.py, which names
# the directories its libraries are in first, since on Windows they are not
# beside it and only os.add_dll_directory() finds them. py.typed is what
# says the stubs beside it are to be trusted.
#
# nanobind_add_stub() would do this, but it runs stubgen directly with no
# way to name those directories.
if(EXISTS ${NB_DIR}/src/stubgen.py)
    set(ftk_STUBGEN ${NB_DIR}/src/stubgen.py)
else()
    set(ftk_STUBGEN ${NB_DIR}/stubgen.py)
endif()
set(ftk_PYTHON_STUB ${ftk_PYTHON_PACKAGE_DIR}/_feather_tk.pyi)
add_custom_command(
    OUTPUT ${ftk_PYTHON_STUB}
    COMMAND ${Python_EXECUTABLE}
        ${PROJECT_SOURCE_DIR}/etc/Python/make_stub.py
        --dll-dir $<TARGET_FILE_DIR:ftkCore>
        --dll-dir $<TARGET_FILE_DIR:ftkGL>
        --dll-dir $<TARGET_FILE_DIR:ftkUI>
        --dll-dir ${CMAKE_INSTALL_PREFIX}/bin
        # SDL, FreeType, libpng and zlib: from the prefix a source build
        # installs them to, or the one a wheel build builds them into,
        # which is not installed yet when this runs.
        --dll-dir ${ftk_WHEEL_DEPS}/bin
        --dll-dir ${ftk_WHEEL_DEPS}/lib
        --
        ${ftk_STUBGEN}
        -q
        -m feather_tk._feather_tk
        -i ${CMAKE_BINARY_DIR}/python
        -o ${ftk_PYTHON_STUB}
    DEPENDS _feather_tk ${ftk_STUBGEN} ${PROJECT_SOURCE_DIR}/etc/Python/make_stub.py
    COMMENT "Generating _feather_tk.pyi")
add_custom_target(_feather_tk_stub ALL DEPENDS ${ftk_PYTHON_STUB})
file(TOUCH ${ftk_PYTHON_PACKAGE_DIR}/py.typed)

# What the stub could not resolve is a C++ type the bindings never exposed;
# the build says so rather than leaving it in the file to be found. See
# etc/Python/check_stub.py.
add_custom_command(
    TARGET _feather_tk_stub POST_BUILD
    COMMAND ${Python_EXECUTABLE}
        ${PROJECT_SOURCE_DIR}/etc/Python/check_stub.py
        ${ftk_PYTHON_STUB})

install(TARGETS _feather_tk
    LIBRARY DESTINATION ${ftk_PYTHON_INSTALL_DIR}
    RUNTIME DESTINATION ${ftk_PYTHON_INSTALL_DIR})
install(
    FILES
        __init__.py
        ${ftk_PYTHON_PACKAGE_DIR}/_feather_tk.pyi
        ${ftk_PYTHON_PACKAGE_DIR}/py.typed
    DESTINATION ${ftk_PYTHON_INSTALL_DIR})
