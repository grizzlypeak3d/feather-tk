ftk_embed_text(
    ${CMAKE_CURRENT_BINARY_DIR}/PanelJson.h
    ${CMAKE_CURRENT_SOURCE_DIR}/panel.json
    panelJson)

add_executable(panel panel.cpp ${CMAKE_CURRENT_BINARY_DIR}/PanelJson.h)

target_include_directories(panel PRIVATE
    ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR} ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(panel ftkUI)

set_target_properties(panel PROPERTIES FOLDER examples)

# The example opens a window, so it cannot run where there is no
# OpenGL. It is still built, which is the compile check.
if(NOT ftk_TESTS_NO_GL)
    add_test(NAME panel COMMAND panel -exit -log)
endif()
