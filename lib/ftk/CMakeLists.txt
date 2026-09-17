add_subdirectory(Core)
if ("${ftk_API}" STREQUAL "GL_4_1" OR
    "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
    "${ftk_API}" STREQUAL "GLES_3")
    add_subdirectory(GL)
endif()
add_subdirectory(Resource)
add_subdirectory(TestLib)
add_subdirectory(UI)

if(ftk_PYTHON)
    # The binding libraries are compiled into the module, so they have to be
    # built against the same nanobind library it is: under the stable ABI that
    # one defines Py_LIMITED_API, which changes nanobind's own headers.
    # nanobind_add_module() finds the library already made and reuses it.
    set(ftk_NANOBIND_LIBRARY nanobind-static)
    if(ftk_PYTHON_STABLE_ABI)
        set(ftk_NANOBIND_LIBRARY nanobind-static-abi3)
    endif()
    nanobind_build_library(${ftk_NANOBIND_LIBRARY})
    if(ftk_PYTHON_STABLE_ABI)
        target_compile_definitions(${ftk_NANOBIND_LIBRARY} PUBLIC Py_LIMITED_API=0x030C0000)
    endif()

    # A wheel leaves out the binding archives: nothing links them but the
    # module, which already has, and together they are larger than it is.
    # Their headers stay, for the modules built on this one.
    set(ftk_PYTHON_ARCHIVE_INSTALL)
    if(SKBUILD)
        set(ftk_PYTHON_ARCHIVE_INSTALL EXCLUDE_FROM_ALL)
    endif()

    add_subdirectory(CorePy)
    add_subdirectory(UIPy)
    add_subdirectory(ftkPy)
endif()

if(ftk_TESTS)
    add_subdirectory(CoreTest)
    if ("${ftk_API}" STREQUAL "GL_4_1" OR
        "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
        "${ftk_API}" STREQUAL "GLES_3")
        add_subdirectory(GLTest)
    endif()
    add_subdirectory(UITest)
endif()
