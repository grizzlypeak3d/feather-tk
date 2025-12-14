set(HEADERS
    Bindings.h)
set(HEADERS_PRIVATE)
set(SOURCE
    Action.cpp
    App.cpp
    Bellows.cpp
    Bindings.cpp
    ButtonGroup.cpp
    CheckBox.cpp
    ColorPopup.cpp
    ColorSwatch.cpp
    ColorWidget.cpp
    ConfirmDialog.cpp
    DialogSystem.cpp
    Divider.cpp
    DoubleEdit.cpp
    DoubleEditSlider.cpp
    DoubleModel.cpp
    DoubleSlider.cpp
    Event.cpp
    FileBrowser.cpp
    FileBrowserWidgets.cpp
    FloatEdit.cpp
    FloatEditSlider.cpp
    FloatModel.cpp
    FloatSlider.cpp
    FileEdit.cpp
    FormLayout.cpp
    GridLayout.cpp
    GroupBox.cpp
    IButton.cpp
    IDialog.cpp
    IMenuPopup.cpp
    IMouseWidget.cpp
    IPopup.cpp
    IWidget.cpp
    IWidgetPopup.cpp
    IWindow.cpp
    Icon.cpp
    IconSystem.cpp
    IntEdit.cpp
    IntEditSlider.cpp
    IntModel.cpp
    IntSlider.cpp
    Label.cpp
    MDICanvas.cpp
    MDIWidget.cpp
    MainWindow.cpp
    Menu.cpp
    MenuBar.cpp
    MessageDialog.cpp
    ProgressDialog.cpp
    PushButton.cpp
    RadioButton.cpp
    RecentFilesModel.cpp
    RowLayout.cpp
    ScrollArea.cpp
    ScrollWidget.cpp
    Settings.cpp
    Splitter.cpp
    StackLayout.cpp
    Style.cpp
    TabBar.cpp
    TabWidget.cpp
    ToolBar.cpp
    ToolButton.cpp
    WidgetOptions.cpp
    Window.cpp)

add_library(ftkUIPy ${HEADERS} ${HEADERS_PRIVATE} ${SOURCE})
target_link_libraries(ftkUIPy ftkUI Python3::Python)
set_target_properties(ftkUIPy PROPERTIES FOLDER lib)
set_target_properties(ftkUIPy PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS ftkUIPy
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/UIPy)
