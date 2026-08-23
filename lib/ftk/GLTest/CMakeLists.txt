set(HEADERS
    MeshTest.h
    OffscreenBufferTest.h
    RenderTest.h
    ShaderTest.h
    TextureAtlasTest.h
    TextureTest.h
    UtilTest.h
    WindowTest.h)
set(PRIVATE_HEADERS)

set(SOURCE
    MeshTest.cpp
    OffscreenBufferTest.cpp
    RenderTest.cpp
    ShaderTest.cpp
    TextureAtlasTest.cpp
    TextureTest.cpp
    UtilTest.cpp
    WindowTest.cpp)

# Static, for the reason in ftk's TestLib.
add_library(ftkGLTest STATIC ${HEADERS} ${PRIVATE_HEADERS} ${SOURCE})
set_target_properties(ftkGLTest PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(ftkGLTest PRIVATE ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR})

target_link_libraries(ftkGLTest ftkGL ftkTestLib)

set_target_properties(ftkGLTest PROPERTIES FOLDER tests)
set_target_properties(ftkGLTest PROPERTIES PUBLIC_HEADER "${HEADERS}")

