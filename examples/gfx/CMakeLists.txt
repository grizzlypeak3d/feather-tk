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

add_test(NAME gfx COMMAND gfx -exit -log)
