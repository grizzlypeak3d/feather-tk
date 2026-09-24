# The notices for what feather-tk itself brings into a build: its own license
# and those of the components it builds or embeds. Each notice lives in one
# repository only -- the one whose build ships the component -- and a project
# that builds on this one takes these from here rather than keeping copies,
# so they cannot drift apart and a component cannot be listed where it is not
# built.
#
# No notice for CMake: nothing here is under its license, and no package
# ships it.
set(LEGAL
    LICENSE_FreeType.txt
    LICENSE_glad.txt
    LICENSE_libpng.txt
    LICENSE_lunasvg.txt
    LICENSE_nlohmann_json.txt
    LICENSE_OFL.txt
    LICENSE_plutovg.txt
    LICENSE_SDL2.txt
    LICENSE_zlib.txt)

# Installed beside the package, which is where a project that finds feather-tk
# as a package reads them from (ftk_LEGAL_DIR in ftkConfig.cmake). The
# project's own license is LICENSE.txt at the top of the repository, installed
# under the name the notices are known by.
install(
    FILES ${LEGAL}
    DESTINATION share/ftk/Legal)
install(
    FILES ${PROJECT_SOURCE_DIR}/LICENSE.txt
    DESTINATION share/ftk/Legal
    RENAME LICENSE_feather-tk.txt)

# And for a project that builds feather-tk as a subdirectory, the same list as
# full paths, collected in a global property: "Name=path" for the license
# whose file name does not say what it is, a path for the rest. DJV writes its
# licenses page from this (docs/Licenses.cmake).
set(ENTRIES "feather-tk=${PROJECT_SOURCE_DIR}/LICENSE.txt")
foreach(FILE ${LEGAL})
    list(APPEND ENTRIES "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}")
endforeach()
set_property(GLOBAL APPEND PROPERTY LEGAL_NOTICES "${ENTRIES}")
