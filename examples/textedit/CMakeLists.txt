set(HEADERS
    Actions.h
    App.h
    Document.h
    DocumentTabs.h
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
    MainWindow.cpp
    MenuBar.cpp
    Settings.cpp
    SettingsModel.cpp
    SettingsWidget.cpp
    StatusBar.cpp
    ToolBar.cpp
    main.cpp)

add_executable(textedit ${HEADERS} ${SOURCE})

target_include_directories(textedit PRIVATE ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR})

target_link_libraries(textedit ftkUI)

set_target_properties(textedit PROPERTIES FOLDER examples)

# The example opens a window, so it cannot run where there is no
# OpenGL. It is still built, which is the compile check.
if(NOT ftk_TESTS_NO_GL)
    add_test(NAME textedit COMMAND textedit -exit -log)
endif()
