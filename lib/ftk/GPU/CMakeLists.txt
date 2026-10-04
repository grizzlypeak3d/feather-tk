set(HEADERS
    Export.h
    OffscreenBuffer.h
    Present.h
    Render.h
    System.h
    Texture.h)
set(PRIVATE_HEADERS
    RenderPrivate.h)

set(SOURCE
    OffscreenBuffer.cpp
    Present.cpp
    Render.cpp
    RenderPrims.cpp
    RenderShaders.cpp
    System.cpp
    Texture.cpp)

add_library(ftkGPU ${HEADERS} ${PRIVATE_HEADERS} ${SOURCE})
if(BUILD_SHARED_LIBS)
    target_compile_definitions(ftkGPU PRIVATE FTK_GPU_EXPORTS)
else()
    target_compile_definitions(ftkGPU PUBLIC FTK_GPU_STATIC)
endif()
target_compile_definitions(ftkGPU PUBLIC FTK_GPU)

target_include_directories(ftkGPU
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(ftkGPU PUBLIC ftkCore SDL3::SDL3)

set_target_properties(ftkGPU PROPERTIES FOLDER lib)
set_target_properties(ftkGPU PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(
    TARGETS ftkGPU
    EXPORT ftkGPUTargets
    ARCHIVE DESTINATION lib COMPONENT dev
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/GPU COMPONENT dev)
install(
    EXPORT ftkGPUTargets
    FILE ftkGPUTargets.cmake
    DESTINATION "lib/cmake/ftk"
    NAMESPACE ftk::
    COMPONENT dev)
