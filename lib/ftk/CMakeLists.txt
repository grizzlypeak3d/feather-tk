add_subdirectory(Core)
add_subdirectory(Resource)
add_subdirectory(UI)

if ("${ftk_API}" STREQUAL "GL_4_1" OR
    "${ftk_API}" STREQUAL "GL_4_1_Debug" OR
    "${ftk_API}" STREQUAL "GLES_2")
    add_subdirectory(GL)
endif()

if(ftk_PYTHON)
    add_subdirectory(CorePy)
    add_subdirectory(UIPy)
    add_subdirectory(ftkPy)
endif()
