set(HEADERS
    App.h)
set(PRIVATE_HEADERS)

set(SOURCE
    App.cpp
    main.cpp)

add_executable(ftk-resource ${HEADERS} ${PRIVATE_HEADERS} ${SOURCE})

target_link_libraries(ftk-resource ZLIB::ZLIB)

set_target_properties(ftk-resource PROPERTIES FOLDER bin)

# Installed only when feather-tk is the project being built. This is a build
# tool -- the libraries run it to turn resources into source -- and inside
# another project the target is already there to be run, so installing it
# only puts a second executable in that project's package, where it has to
# be signed and notarized like an application.
if(PROJECT_IS_TOP_LEVEL)
    set(RUNTIME_DESTINATION "bin")
    install(TARGETS ftk-resource
        EXPORT ftk-resourceTargets
        RUNTIME DESTINATION ${RUNTIME_DESTINATION})
    install(
        EXPORT ftk-resourceTargets
        FILE ftk-resourceTargets.cmake
        DESTINATION "lib/cmake/ftk"
        NAMESPACE ftk::
        COMPONENT dev)
endif()
