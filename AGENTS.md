# feather-tk for agents

Instructions for a coding agent working on feather-tk or building an
application with it. People are welcome to read it too.

feather-tk is a C++ (CMake, C++20) user interface toolkit for media
production tools, with Python bindings. It knows nothing about timelines
or media files: those belong to tlRender, which is built on it, and DJV is
built on both. Code that needs a timeline, a player or a file format goes
there, not here.

## Where things are

- `lib/ftk/Core`: everything without a window. Before writing a small
  utility, look here; most of the common ones exist:
  - `String.h`: split, join, quotes, case, elide, wildcard matching,
    UTF-8 and wide strings.
  - `Format.h`: `ftk::Format("{0} of {1}").arg(a).arg(b)`, the way
    messages are built.
  - `Path.h`: file paths and image sequences (`expandSeq`).
  - `OS.h`: system information, environment variables, `openURL`,
    `isNetworkVolume`, thread names.
  - `FileIO.h`, `Time.h`, `Timer.h`, `Math.h`, `Range.h`,
    `Box.h`, `Vector.h`, `Size.h`, `Color.h`, `Image.h`, `Memory.h`,
    `LRUCache.h`, `Random.h`.
  - `Observable.h`, `ObservableList.h`, `ObservableMap.h`: the values
    that widgets and models watch, with an `Observer` that stops when it
    is destroyed.
  - `Context.h` and the systems (`LogSystem`, `FontSystem`, ...), and
    `CmdLine.h` for an application's options.
- `lib/ftk/UI`: widgets, layouts, windows, `Style` (sizes and colors by
  role, never fixed values), `Action` (one command for a menu, a tool bar
  button and a shortcut), `Settings`, `Capture` (screenshots driven by a
  JSON manifest, for tests and documentation).
- `lib/ftk/GL`, `lib/ftk/GPU`: drawing through OpenGL, and the
  experimental SDL GPU renderer.
- `lib/ftk/*Py`, `ftkPy`: the Python bindings (nanobind), the
  `feather_tk` module.
- `lib/ftk/Resource`, `etc/Icons`, `etc/Fonts`: icons and fonts, embedded
  into the library at build time.
- `examples`: small applications, also built for the web.
- `etc/SuperBuild`: how each dependency is built, with its version and
  any patches.

## The macros

In `Core/Util.h` unless noted. Grep does not find what they declare, so
it helps to know them:

- `FTK_ENUM(API, Name)` declares, and `FTK_ENUM_IMPL(Name, ...)` defines
  with the labels, `getNameEnums()`, `getNameLabels()`, `getLabel(Name)`,
  `to_string`, `from_string` and `operator <<` for an enum.
- `FTK_PRIVATE()` declares a class's private implementation, `_p`;
  `FTK_P()` in a member function makes `p` a reference to it.
- `FTK_NON_COPYABLE(Class)` deletes the copy constructor and assignment.
- `FTK_ASSERT` (`Assert.h`, compiled out in Release) and `FTK_CHECK`
  (`TestLib/ITest.h`, for tests).
- `FTK_MAIN()` (`IApp.h`) is `main`, or `wmain` on Windows.

## Building and testing

The super build builds the dependencies; feather-tk is then built against
them. README.md has the commands for each platform. The tests are on by
default (`ftk_TESTS`); Python is `-Dftk_PYTHON=ON`. Then, in the build
directory:

```
ctest -j6
```

`ftk-test` takes the names of the tests to run, and `-noGL` runs only the
ones that need no OpenGL. A test build that was not rebuilt runs old
binaries, so check that the test executables are newer than your change.

## Changing feather-tk

- Widgets are made with `create()`, held by `std::shared_ptr`, and given
  a parent; state a widget shows comes from an observable or a setter,
  not from the widget reaching into the application.
- Windows needs an export macro (`FTK_CORE_API`, `FTK_UI_API`, ...) on
  every function used from another library, protected ones included when
  a class is meant to be subclassed there.
- Python mirrors the C++ API: a public header change usually needs its
  binding.

## Contributions

- Tests with every change, and the existing tests passing.
- One change per pull request.
- No new dependencies without discussing them first.
- The person who sends a change stands behind it, however it was written.
