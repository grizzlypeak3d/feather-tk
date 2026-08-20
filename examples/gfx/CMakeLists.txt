set(HEADERS
    App.h
    GOL.h
    MainWindow.h
    Noise.h)

set(SOURCE
    App.cpp
    GOL.cpp
    MainWindow.cpp
    Noise.cpp
    main.cpp)

add_executable(gfx ${HEADERS} ${SOURCE})

target_include_directories(gfx PRIVATE ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR})

target_link_libraries(gfx ftkUI)

set_target_properties(gfx PROPERTIES FOLDER examples)

# The example opens a window, so it cannot run where there is no
# OpenGL. It is still built, which is the compile check.
if(NOT ftk_TESTS_NO_GL)
    add_test(NAME gfx COMMAND gfx -exit -log)
endif()
