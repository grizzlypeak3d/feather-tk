set(HEADERS
    App.h
    Bellows.h
    Buttons.h
    Charts.h
    Dialogs.h
    DragDrop.h
    Graphs.h
    Icons.h
    Images.h
    Layouts.h
    Lists.h
    MDI.h
    MainWindow.h
    Offscreen.h
    Popups.h
    Sliders.h
    Splitters.h
    Stack.h
    SysLog.h)

set(SOURCE
    App.cpp
    Bellows.cpp
    Buttons.cpp
    Charts.cpp
    Dialogs.cpp
    DragDrop.cpp
    Graphs.cpp
    Icons.cpp
    Images.cpp
    Layouts.cpp
    Lists.cpp
    MDI.cpp
    MainWindow.cpp
    Offscreen.cpp
    Popups.cpp
    Sliders.cpp
    Splitters.cpp
    Stack.cpp
    SysLog.cpp
    main.cpp)

add_executable(widgets ${HEADERS} ${SOURCE})
target_link_libraries(widgets ftkUI)
set_target_properties(widgets PROPERTIES FOLDER examples)

add_test(NAME widgets COMMAND widgets -exit -log)
