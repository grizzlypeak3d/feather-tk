set(HEADERS
    ftk/glad/gl.h
    ftk/KHR/khrplatform.h)

set(SOURCE
    src/gl.c)

add_library(ftkglad ${HEADERS} ${SOURCE})
target_link_libraries(ftkglad OpenGL::GL ${CMAKE_DL_LIBS})
set_target_properties(ftkglad PROPERTIES FOLDER deps)

install(
    TARGETS ftkglad
    EXPORT ftkgladTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(
    EXPORT ftkgladTargets
    FILE ftkgladTargets.cmake
    DESTINATION "lib/cmake/ftk"
    NAMESPACE ftk::)
install(
    DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/ftk/glad"
    DESTINATION "include/ftk")
install(
    DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/ftk/KHR"
    DESTINATION "include/ftk")
