set(HEADERS ITest.h)
set(PRIVATE_HEADERS)

set(SOURCE ITest.cpp)

add_library(ftkTestLib ${HEADERS} ${PRIVATE_HEADERS} ${SOURCE})
target_link_libraries(ftkTestLib PUBLIC ftkCore)
set_target_properties(ftkTestLib PROPERTIES FOLDER tests)
set_target_properties(ftkTestLib PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(
    TARGETS ftkTestLib
    EXPORT ftkTestLibTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/TestLib)
install(
    EXPORT ftkTestLibTargets
    FILE ftkTestLibTargets.cmake
    DESTINATION "lib/cmake/ftk"
    NAMESPACE ftk::)
