set(HEADERS
    Bellows.h
    Buttons.h
    Charts.h
    Dialogs.h
    DragDrop.h
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
    Stack.h)

set(SOURCE
    Bellows.cpp
    Buttons.cpp
    Charts.cpp
    Dialogs.cpp
    DragDrop.cpp
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
    main.cpp)

add_executable(widgets ${HEADERS} ${SOURCE})
target_link_libraries(widgets ftkUI)
set_target_properties(widgets PROPERTIES FOLDER examples)

add_test(NAME widgets COMMAND widgets -exit -log)
