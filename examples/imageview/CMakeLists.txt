set(HEADERS
    Actions.h
    App.h
    Document.h
    DocumentTabs.h
    ImageView.h
    MainWindow.h
    MenuBar.h
    Settings.h
    SettingsModel.h
    SettingsWidget.h
    StatusBar.h
    ToolBar.h)

set(SOURCE
    Actions.cpp
    App.cpp
    Document.cpp
    DocumentTabs.cpp
    ImageView.cpp
    MainWindow.cpp
    MenuBar.cpp
    Settings.cpp
    SettingsModel.cpp
    SettingsWidget.cpp
    StatusBar.cpp
    ToolBar.cpp
    main.cpp)

add_executable(imageview ${HEADERS} ${SOURCE})

target_include_directories(imageview PRIVATE ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR})

target_link_libraries(imageview ftkUI)

set_target_properties(imageview PROPERTIES FOLDER examples)

# The example opens a window, so it cannot run where there is no
# OpenGL. It is still built, which is the compile check.
if(NOT ftk_TESTS_NO_GL)
    add_test(NAME imageview COMMAND imageview -exit -log)
endif()
