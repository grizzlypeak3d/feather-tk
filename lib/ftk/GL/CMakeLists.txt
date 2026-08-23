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
# Private when building, so this library's own API is dllexport here and
# dllimport to everything else. Public when static, which a consumer does
# have to know: the API then carries no declspec at all.
if(BUILD_SHARED_LIBS)
    target_compile_definitions(ftkGL PRIVATE FTK_GL_EXPORTS)
else()
    target_compile_definitions(ftkGL PUBLIC FTK_GL_STATIC)
endif()

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

target_include_directories(ftkGL
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
        $<BUILD_INTERFACE:${ftk_GLAD_DIR}>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(ftkGL PUBLIC ftkCore ftkglad)
# Public because this is not the only library that calls SDL: the UI does,
# for the window and the clipboard, and so does tlRender for the audio. A
# private link reaches them anyway while everything is static -- the symbols
# wait for the program, or a static SDL is folded into this library and
# re-exported -- and stops reaching them the moment either becomes shared.
if(ftk_SDL2)
    target_link_libraries(ftkGL PUBLIC SDL2::SDL2)
elseif(ftk_SDL3)
    target_link_libraries(ftkGL PUBLIC SDL3::SDL3)
endif()

set_target_properties(ftkGL PROPERTIES FOLDER lib)
set_target_properties(ftkGL PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(
    TARGETS ftkGL
    EXPORT ftkGLTargets
    ARCHIVE DESTINATION lib COMPONENT dev
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/GL COMPONENT dev)
install(
    EXPORT ftkGLTargets
    FILE ftkGLTargets.cmake
    DESTINATION "lib/cmake/ftk"
    NAMESPACE ftk::
    COMPONENT dev)
