set(HEADERS
    Actions.h
    App.h
    Document.h
    DocumentTabs.h
    HUDWidget.h
    MainWindow.h
    MenuBar.h
    ObjIO.h
    ObjView.h
    Settings.h
    SettingsModel.h
    SettingsWidget.h
    ToolBar.h)

set(SOURCE
    Actions.cpp
    App.cpp
    Document.cpp
    DocumentTabs.cpp
    HUDWidget.cpp
    MainWindow.cpp
    MenuBar.cpp
    ObjIO.cpp
    ObjView.cpp
    Settings.cpp
    SettingsModel.cpp
    SettingsWidget.cpp
    ToolBar.cpp
    main.cpp)

add_executable(objview ${HEADERS} ${SOURCE})

target_include_directories(objview PRIVATE ${PROJECT_SOURCE_DIR}/lib ${ftk_GLAD_DIR})

target_link_libraries(objview ftkUI)

set_target_properties(objview PROPERTIES FOLDER examples)

# The example opens a window, so it cannot run where there is no
# OpenGL. It is still built, which is the compile check.
if(NOT ftk_TESTS_NO_GL)
    add_test(NAME objview COMMAND objview -exit -log)
endif()
