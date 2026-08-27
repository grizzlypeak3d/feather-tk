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

if(EMSCRIPTEN)
    # The page asks for its .js and .wasm with the build id, so a cached
    # page always gets the pair it was built with -- a page and a binary
    # from different deployments abort on load.
    execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE ftk_WEB_ID
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET)
    if(NOT ftk_WEB_ID)
        set(ftk_WEB_ID ${CMAKE_PROJECT_VERSION})
    endif()
endif()

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
