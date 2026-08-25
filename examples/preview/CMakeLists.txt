ftk_embed_text(
    ${CMAKE_CURRENT_BINARY_DIR}/PreviewJson.h
    ${CMAKE_CURRENT_SOURCE_DIR}/preview.json
    previewJson)

add_executable(preview preview.cpp ${CMAKE_CURRENT_BINARY_DIR}/PreviewJson.h)

target_include_directories(preview PRIVATE
    ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR} ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(preview ftkUI)

set_target_properties(preview PROPERTIES FOLDER examples)

# The example opens a window, so it cannot run where there is no
# OpenGL. It is still built, which is the compile check.
if(NOT ftk_TESTS_NO_GL)
    add_test(NAME preview
        COMMAND preview ${CMAKE_CURRENT_SOURCE_DIR}/preview.json -exit -log)
    # No argument shows the embedded sample.
    add_test(NAME preview-embedded COMMAND preview -exit -log)
endif()
