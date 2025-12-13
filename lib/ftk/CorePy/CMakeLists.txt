set(HEADERS
    Bindings.h
    CmdLineInline.h
    ObservableInline.h
    ObservableListInline.h
    ObservableMapInline.h)
set(HEADERS_PRIVATE)
set(SOURCE
    Box.cpp
    Bindings.cpp
    CmdLine.cpp
    Color.cpp
    Context.cpp
    FontSystem.cpp
    IApp.cpp
    IRender.cpp
    ISystem.cpp
    Image.cpp
    LogSystem.cpp
    Memory.cpp
    OS.cpp
    Observable.cpp
    ObservableList.cpp
    ObservableMap.cpp
    Path.cpp
    Random.cpp
    Range.cpp
    Size.cpp
    String.cpp
    Timer.cpp
    Vector.cpp)

add_library(ftkCorePy ${HEADERS} ${HEADERS_PRIVATE} ${SOURCE})
target_link_libraries(ftkCorePy ftkCore Python3::Python)
set_target_properties(ftkCorePy PROPERTIES FOLDER lib)
set_target_properties(ftkCorePy PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS ftkCorePy
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/CorePy)
