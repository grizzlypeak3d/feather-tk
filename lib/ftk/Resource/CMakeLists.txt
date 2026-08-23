set(HEADERS)
set(SOURCE)
set(RESOURCES
    etc/Fonts/NotoMono-Regular.ttf
    etc/Fonts/NotoSans-Bold.ttf
    etc/Fonts/NotoSans-Regular.ttf
    etc/Fonts/NotoSansSymbols2-Regular.ttf
    etc/Fonts/NotoSansCJK-Regular.otf
    etc/Icons/ArrowDown.svg
    etc/Icons/ArrowLeft.svg
    etc/Icons/ArrowRight.svg
    etc/Icons/ArrowUp.svg
    etc/Icons/Audio.svg
    etc/Icons/BellowsClosed.svg
    etc/Icons/BellowsOpen.svg
    etc/Icons/Clear.svg
    etc/Icons/ClearSmall.svg
    etc/Icons/Close.svg
    etc/Icons/CloseSmall.svg
    etc/Icons/Copy.svg
    etc/Icons/Cut.svg
    etc/Icons/Decrement.svg
    etc/Icons/Directory.svg
    etc/Icons/DirectoryBack.svg
    etc/Icons/DirectoryForward.svg
    etc/Icons/DirectoryUp.svg
    etc/Icons/Edit.svg
    etc/Icons/Empty.svg
    etc/Icons/File.svg
    etc/Icons/FileBrowser.svg
    etc/Icons/FileClose.svg
    etc/Icons/FileCloseAll.svg
    etc/Icons/FileNew.svg
    etc/Icons/FileOpen.svg
    etc/Icons/FileReload.svg
    etc/Icons/FileSave.svg
    etc/Icons/FrameEnd.svg
    etc/Icons/FrameInOut.svg
    etc/Icons/FrameNext.svg
    etc/Icons/FramePrev.svg
    etc/Icons/FrameStart.svg
    etc/Icons/Increment.svg
    etc/Icons/Info.svg
    etc/Icons/MenuArrow.svg
    etc/Icons/MenuChecked.svg
    etc/Icons/Mute.svg
    etc/Icons/Next.svg
    etc/Icons/PanelBottom.svg
    etc/Icons/PanelLeft.svg
    etc/Icons/PanelRight.svg
    etc/Icons/PanelTop.svg
    etc/Icons/Paste.svg
    etc/Icons/PlaybackForward.svg
    etc/Icons/PlaybackReverse.svg
    etc/Icons/PlaybackStop.svg
    etc/Icons/Prev.svg
    etc/Icons/Redo.svg
    etc/Icons/Reload.svg
    etc/Icons/Reset.svg
    etc/Icons/ReverseSort.svg
    etc/Icons/Search.svg
    etc/Icons/Settings.svg
    etc/Icons/Shuttle0.svg
    etc/Icons/Shuttle1.svg
    etc/Icons/Shuttle2.svg
    etc/Icons/Shuttle3.svg
    etc/Icons/Shuttle4.svg
    etc/Icons/Shuttle5.svg
    etc/Icons/Shuttle6.svg
    etc/Icons/Shuttle7.svg
    etc/Icons/SubMenuArrow.svg
    etc/Icons/Time.svg
    etc/Icons/Undo.svg
    etc/Icons/ViewFrame.svg
    etc/Icons/ViewZoomIn.svg
    etc/Icons/ViewZoomOut.svg
    etc/Icons/ViewZoomReset.svg
    etc/Icons/Volume.svg
    etc/Icons/WindowFullScreen.svg
    etc/Icons/feather_tk_512.svg
    etc/Images/Charlie.png
    etc/Images/TestDrawing.svg)
foreach(RESOURCE ${RESOURCES})
    get_filename_component(RESOURCE_BASE ${RESOURCE} NAME_WE)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_BASE}.cpp
        COMMAND ftk-resource ${PROJECT_SOURCE_DIR}/${RESOURCE} ${RESOURCE_BASE} ftk_resource
        DEPENDS ${PROJECT_SOURCE_DIR}/${RESOURCE})
    list(APPEND SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_BASE}.cpp)
endforeach()

# Static, whatever the rest of the build is. The resources are data at
# namespace scope with no export macro on them, so a shared build on Windows
# produces a library that exports nothing and no import library beside it:
#
#     LINK : fatal error LNK1104: cannot open file 'ftkResource.lib'
#
# CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS does not answer this the way it does for
# the dependencies: it writes a module definition file from the functions the
# objects hold, and these hold none. Marking the data instead would mean the
# generator and every extern declaration that names it.
#
# Static suits them anyway. They are an implementation detail rather than API,
# and each resource is its own object file, so a consumer links only the ones
# it names -- FontSystem and IconSystem take the ones they use and nothing more.
add_library(ftkResource STATIC ${HEADERS} ${SOURCE})
set_target_properties(ftkResource PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(ftkResource PUBLIC FTK_CJK)

target_link_libraries(ftkResource ZLIB::ZLIB)

set_target_properties(ftkResource PROPERTIES FOLDER lib)
set_target_properties(ftkResource PROPERTIES PUBLIC_HEADER "${HEADERS}")

add_dependencies(ftkResource ftk-resource)

install(
    TARGETS ftkResource
    EXPORT ftkResourceTargets
    ARCHIVE DESTINATION lib COMPONENT dev
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/Resource COMPONENT dev)
install(
    EXPORT ftkResourceTargets
    FILE ftkResourceTargets.cmake
    DESTINATION "lib/cmake/ftk"
    NAMESPACE ftk::
    COMPONENT dev)
