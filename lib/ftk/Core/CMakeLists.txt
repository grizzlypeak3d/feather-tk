set(HEADERS
    Assert.h
    Box.h
    BoxInline.h
    BoxPack.h
    CmdLineInline.h
    CmdLine.h
    Color.h
    ColorInline.h
    Command.h
    Context.h
    ContextInline.h
    Error.h
    FileIO.h
    FileIOInline.h
    FontSystem.h
    FontSystemInline.h
    Format.h
    FormatInline.h
    IApp.h
    IRender.h
    ISystem.h
    ISystemInline.h
    ImageIO.h
    Image.h
    ImageInline.h
    LogSystem.h
    LRUCache.h
    LRUCacheInline.h
    Math.h
    MathInline.h
    Matrix.h
    MatrixInline.h
    Memory.h
    MemoryInline.h
    Mesh.h
    MeshInline.h
    Noise.h
    OS.h
    Observable.h
    ObservableInline.h
    ObservableList.h
    ObservableListInline.h
    ObservableMap.h
    ObservableMapInline.h
    PNG.h
    Path.h
    PathInline.h
    Random.h
    RandomInline.h
    Range.h
    RangeInline.h
    RenderOptions.h
    RenderOptionsInline.h
    RenderUtil.h
    Size.h
    SizeInline.h
    String.h
    Time.h
    Timer.h
    Util.h
    Vector.h
    VectorInline.h)
set(HEADERS_PRIVATE
    PNGPrivate.h)
set(SOURCE
    Assert.cpp
    Box.cpp
    BoxPack.cpp
    CmdLine.cpp
    Command.cpp
    Color.cpp
    Context.cpp
    Error.cpp
    FileIO.cpp
    FontSystem.cpp
    Format.cpp
    IApp.cpp
    IRender.cpp
    ISystem.cpp
    ImageIO.cpp
    Image.cpp
    LogSystem.cpp
    Math.cpp
    Matrix.cpp
    Memory.cpp
    Mesh.cpp
    Noise.cpp
    PNG.cpp
    PNGRead.cpp
    PNGWrite.cpp
    Path.cpp
    OS.cpp
    Random.cpp
    Range.cpp
    RenderOptions.cpp
    RenderUtil.cpp
    Size.cpp
    String.cpp
    Time.cpp
    Timer.cpp
    Vector.cpp)
if(WIN32)
    list(APPEND SOURCE
        ErrorWin32.cpp
        FileIOWin32.cpp
        OSWin32.cpp
        PathWin32.cpp
        TimeWin32.cpp)
else()
    list(APPEND SOURCE
        FileIOUnix.cpp
        OSUnix.cpp
        PathUnix.cpp
        TimeUnix.cpp)
    if(APPLE)
        list(APPEND SOURCE
            PathMacOS.mm)
    endif()
endif()

set(LIBS_PUBLIC ftkResource nlohmann_json::nlohmann_json)
if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    list(APPEND LIBS_PUBLIC stdc++fs)
endif()
set(LIBS_PRIVATE Freetype::Freetype PNG::PNG ZLIB::ZLIB)
if(APPLE)
    list(APPEND LIBS_PRIVATE "-framework CoreServices" "-framework Foundation")
endif()

add_library(ftkCore ${HEADERS} ${HEADERS_PRIVATE} ${SOURCE})
target_link_libraries(ftkCore PUBLIC ${LIBS_PUBLIC} PRIVATE ${LIBS_PRIVATE})
set_target_properties(ftkCore PROPERTIES FOLDER lib)
set_target_properties(ftkCore PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(
    TARGETS ftkCore
    EXPORT ftkCoreTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/ftk/Core)
install(
    EXPORT ftkCoreTargets
    FILE ftkCoreTargets.cmake
    DESTINATION "share/ftk"
    NAMESPACE ftk::)
