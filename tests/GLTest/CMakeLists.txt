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

add_library(ftkGLTest ${HEADERS} ${PRIVATE_HEADERS} ${SOURCE})

target_include_directories(ftkGLTest PRIVATE ${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/tests ${ftk_GLAD_DIR})

target_link_libraries(ftkGLTest ftkGL ftkTestLib)

set_target_properties(ftkGLTest PROPERTIES FOLDER tests)
set_target_properties(ftkGLTest PROPERTIES PUBLIC_HEADER "${HEADERS}")

