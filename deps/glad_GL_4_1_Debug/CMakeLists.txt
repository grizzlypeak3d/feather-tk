set(HEADERS
    ftk/glad/gl.h
    ftk/KHR/khrplatform.h)

set(SOURCE
    src/gl.c)

# Static, whatever the rest of the build is. glad is a generated OpenGL loader
# in C with no export macros on it, so a shared build on Windows produces a
# library that exports nothing and no import library beside it:
#
#     LINK : fatal error LNK1104: cannot open file 'ftkglad.lib'
#
# It is an implementation detail of ftkGL rather than API, and its symbols are
# resolved at load time from the system's OpenGL anyway, so there is nothing
# to gain by giving it a library of its own.
add_library(ftkglad STATIC ${HEADERS} ${SOURCE})
set_target_properties(ftkglad PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(ftkglad PRIVATE ${PROJECT_SOURCE_DIR}/deps/glad_GL_4_1_Debug)

target_link_libraries(ftkglad OpenGL::GL ${CMAKE_DL_LIBS})

set_target_properties(ftkglad PROPERTIES FOLDER deps)

install(
    TARGETS ftkglad
    EXPORT ftkgladTargets
    ARCHIVE DESTINATION lib COMPONENT dev
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(
    EXPORT ftkgladTargets
    FILE ftkgladTargets.cmake
    DESTINATION "lib/cmake/ftk"
    NAMESPACE ftk::
    COMPONENT dev)
install(
    DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/ftk/glad"
    DESTINATION "include/ftk"
    COMPONENT dev)
install(
    DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/ftk/KHR"
    DESTINATION "include/ftk"
    COMPONENT dev)
