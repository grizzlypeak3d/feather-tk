set(LEGAL
    LICENSE_CMake.txt
    LICENSE_FreeType.txt
    LICENSE_feather-tk.txt
    LICENSE_glad.txt
    LICENSE_libpng.txt
    LICENSE_lunasvg.txt
    LICENSE_nlohmann_json.txt
    LICENSE_OFL.txt
    LICENSE_plutovg.txt
    LICENSE_SDL2.txt
    LICENSE_zlib.txt)

install(
    FILES ${LEGAL}
    DESTINATION share/ftk/Legal)
