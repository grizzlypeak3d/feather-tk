set(HEADERS
    Bindings.h)
set(HEADERS_PRIVATE)
set(SOURCE
    Action.cpp
    ActionGroup.cpp
    App.cpp
    Bellows.cpp
    Bindings.cpp
    ButtonGroup.cpp
    CheckBox.cpp
    ClipboardSystem.cpp
    ColorPopup.cpp
    ColorSlider.cpp
    ColorSwatch.cpp
    ColorWidget.cpp
    ComboBox.cpp
    ConfirmDialog.cpp
    DiagWidget.cpp
    DialogSystem.cpp
    Divider.cpp
    DocumentModel.cpp
    DoubleEdit.cpp
    DoubleEditShuttle.cpp
    DoubleEditSlider.cpp
    DoubleModel.cpp
    DoubleSlider.cpp
    DrivesModel.cpp
    Event.cpp
    FileBrowser.cpp
    FileBrowserWidgets.cpp
    FloatEdit.cpp
    FloatEditShuttle.cpp
    FloatEditSlider.cpp
    FloatModel.cpp
    FloatSlider.cpp
    FlowLayout.cpp
    FileEdit.cpp
    FormLayout.cpp
    GraphWidget.cpp
    GridLayout.cpp
    GroupBox.cpp
    IButton.cpp
    IContainer.cpp
    IDialog.cpp
    IMenuPopup.cpp
    IMouseWidget.cpp
    IncButtons.cpp
    IPopup.cpp
    IWidget.cpp
    IWidgetPopup.cpp
    IWindow.cpp
    Icon.cpp
    IconSystem.cpp
    ImageWidget.cpp
    IntEdit.cpp
    IntEditShuttle.cpp
    IntEditSlider.cpp
    IntModel.cpp
    IntSlider.cpp
    Label.cpp
    LevelsSlider.cpp
    LineEdit.cpp
    LineEditModel.cpp
    ListItemsWidget.cpp
    ListWidget.cpp
    MDICanvas.cpp
    MDIMiniMap.cpp
    MDIWidget.cpp
    MainWindow.cpp
    Menu.cpp
    MenuBar.cpp
    MessageDialog.cpp
    OverlayLayout.cpp
    PieChart.cpp
    ProgressDialog.cpp
    PushButton.cpp
    RadioButton.cpp
    RecentFilesModel.cpp
    RowLayout.cpp
    ScreenshotTag.cpp
    SVGWidget.cpp
    SysLogModel.cpp
    ScrollArea.cpp
    ScrollBar.cpp
    ScrollWidget.cpp
    SearchBox.cpp
    Settings.cpp
    ShuttleWidget.cpp
    Spacer.cpp
    Splitter.cpp
    Splitter2D.cpp
    StackLayout.cpp
    Style.cpp
    TabBar.cpp
    TabWidget.cpp
    TextEdit.cpp
    TextEditModel.cpp
    ToolBar.cpp
    ToolButton.cpp
    Tooltip.cpp
    WidgetOptions.cpp
    Window.cpp)

add_library(ftkUIPy ${HEADERS} ${HEADERS_PRIVATE} ${SOURCE})

target_include_directories(ftkUIPy PRIVATE ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(ftkUIPy ftkUI Python3::Python)

set_target_properties(ftkUIPy PROPERTIES FOLDER lib)
set_target_properties(ftkUIPy PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS ftkUIPy
    ARCHIVE DESTINATION lib COMPONENT dev
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/UIPy COMPONENT dev)
