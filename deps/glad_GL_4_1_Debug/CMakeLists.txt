set(HEADERS
    ftk/glad/gl.h
    ftk/KHR/khrplatform.h)

set(SOURCE
    src/gl.c)

# glad is compiled into ftkGL rather than being a library of its own. It is a
# table of function pointers filled in at run time by initGLAD, so a second
# copy of it in a process is a second table that nobody fills: the calls
# through it go to a null pointer. Built as a static library and linked by two
# libraries, that is exactly what happened -- ftkGL initialised its copy and
# tlGL crashed on its own.
#
# One target compiles it, and its symbols are exported from there, so there is
# one table and no way to acquire another.
set(ftk_GLAD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/gl.c PARENT_SCOPE)

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
