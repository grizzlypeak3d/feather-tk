set BUILD_TYPE=%1
IF "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set JOBS=4
set FTK_API=GL_4_1
set FTK_SDL2=ON
set FTK_SDL3=OFF
set FTK_NFD=OFF
set FTK_PYTHON=OFF
set FTK_TESTS=ON
set FTK_EXAMPLES=ON
set FTK_GCOV=OFF
set BUILD_SHARED_LIBS=OFF

feather-tk\etc\Windows\windows-build-gha.bat %BUILD_TYPE%
