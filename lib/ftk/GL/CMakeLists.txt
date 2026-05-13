set(HEADERS
    GL.h
    Init.h
    Mesh.h
    OffscreenBuffer.h
    Render.h
    Shader.h
    System.h
    Texture.h
    TextureAtlas.h
    Util.h
    Window.h)
set(PRIVATE_HEADERS
    RenderPrivate.h)

set(SOURCE
    Init.cpp
    Mesh.cpp
    Mesh.cpp
    OffscreenBuffer.cpp
    Render.cpp
    RenderPrims.cpp
    Shader.cpp
    System.cpp
    Texture.cpp
    TextureAtlas.cpp
    Util.cpp
    Window.cpp)
if ("${ftk_API}" STREQUAL "GL_4_1" OR
    "${ftk_API}" STREQUAL "GL_4_1_Debug")
    list(APPEND SOURCE RenderShaders_GL_4_1.cpp)
elseif ("${ftk_API}" STREQUAL "GLES_2")
    list(APPEND SOURCE RenderShaders_GLES_2.cpp)
endif()

add_library(ftkGL ${HEADERS} ${PRIVATE_HEADERS} ${SOURCE})

if(${ftk_API} STREQUAL "GL_4_1_Debug")
    target_compile_definitions(ftkGL PUBLIC FTK_API_GL_4_1)
else()
    target_compile_definitions(ftkGL PUBLIC FTK_API_${ftk_API})
endif()
if(ftk_SDL2)
    target_compile_definitions(ftkGL PUBLIC FTK_SDL2)
elseif(ftk_SDL3)
    target_compile_definitions(ftkGL PUBLIC FTK_SDL3)
endif()

target_include_directories(ftkGL PRIVATE ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR})

target_link_libraries(ftkGL PUBLIC ftkCore ftkglad)
if(ftk_SDL2)
    target_link_libraries(ftkGL PRIVATE SDL2::SDL2)
elseif(ftk_SDL3)
    target_link_libraries(ftkGL PRIVATE SDL3::SDL3)
endif()

set_target_properties(ftkGL PROPERTIES FOLDER lib)
set_target_properties(ftkGL PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(
    TARGETS ftkGL
    EXPORT ftkGLTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/GL)
install(
    EXPORT ftkGLTargets
    FILE ftkGLTargets.cmake
    DESTINATION "lib/cmake/ftk"
    NAMESPACE ftk::)
