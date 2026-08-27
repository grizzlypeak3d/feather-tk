set(ftk_EXAMPLES
    gfx
    imageview
    objview
    panel
    preview
    simple
    textedit
    widgets
    windows)

foreach(ftk_WEB_APP ${ftk_EXAMPLES})
    add_subdirectory(${ftk_WEB_APP})
    if(EMSCRIPTEN)
        # Each example gets a page to run in; serve the build directory
        # with a local web server, e.g. "python3 -m http.server".
        configure_file(
            ${PROJECT_SOURCE_DIR}/etc/Web/index.html.in
            ${CMAKE_CURRENT_BINARY_DIR}/${ftk_WEB_APP}/index.html
            @ONLY)
    endif()
endforeach()
