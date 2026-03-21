set(HEADERS
    App.h)
set(PRIVATE_HEADERS)

set(SOURCE
    App.cpp
    main.cpp)

add_executable(ftk-resource ${HEADERS} ${PRIVATE_HEADERS} ${SOURCE})
target_link_libraries(ftk-resource)
set_target_properties(ftk-resource PROPERTIES FOLDER bin)

set(RUNTIME_DESTINATION "bin")
install(TARGETS ftk-resource
    EXPORT ftk-resourceTargets
    RUNTIME DESTINATION ${RUNTIME_DESTINATION})
install(
    EXPORT ftk-resourceTargets
    FILE ftk-resourceTargets.cmake
    DESTINATION "share/ftk"
    NAMESPACE ftk::)
