set(HEADERS ITest.h)
set(PRIVATE_HEADERS)

set(SOURCE ITest.cpp)

# Static. The test libraries are linked only by the test programs beside them
# and carry no export macros, so a shared build on Windows would give each one
# a library that exports nothing and no import library to link against.
add_library(ftkTestLib STATIC ${HEADERS} ${PRIVATE_HEADERS} ${SOURCE})
set_target_properties(ftkTestLib PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(ftkTestLib PRIVATE ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(ftkTestLib PUBLIC ftkCore)

set_target_properties(ftkTestLib PROPERTIES FOLDER tests)
set_target_properties(ftkTestLib PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(
    TARGETS ftkTestLib
    EXPORT ftkTestLibTargets
    ARCHIVE DESTINATION lib COMPONENT dev
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/TestLib COMPONENT dev)
install(
    EXPORT ftkTestLibTargets
    FILE ftkTestLibTargets.cmake
    DESTINATION "lib/cmake/ftk"
    NAMESPACE ftk::
    COMPONENT dev)
