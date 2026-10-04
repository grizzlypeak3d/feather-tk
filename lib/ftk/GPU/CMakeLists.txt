set(HEADERS
    Export.h
    OffscreenBuffer.h
    Present.h
    Render.h
    Shader.h
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
    Shader.cpp
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

# glslang compiles GLSL to SPIR-V, which is what Vulkan takes. Metal compiles
# its own language, so on macOS this is only for checking the GLSL: wanted
# where there is Vulkan, and used where it is found.
find_package(glslang CONFIG QUIET)
if(glslang_FOUND)
    target_compile_definitions(ftkGPU PRIVATE FTK_GLSLANG)
    target_link_libraries(ftkGPU PRIVATE
        glslang::glslang
        glslang::glslang-default-resource-limits)
    if(TARGET glslang::SPIRV)
        target_link_libraries(ftkGPU PRIVATE glslang::SPIRV)
    endif()
    message(STATUS "ftkGPU: glslang ${glslang_VERSION}")
elseif(NOT APPLE)
    message(WARNING "ftkGPU: glslang was not found, so there are no shaders for Vulkan")
endif()

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
