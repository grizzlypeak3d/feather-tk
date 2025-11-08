set BUILD_TYPE=%1
IF "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set FTK_API=GL_4_1
set FTK_NFD=OFF
set FTK_PYTHON=OFF
set FTK_TESTS=ON
set FTK_EXAMPLES=ON
set FTK_GCOV=OFF

feather-tk\etc\Windows\windows-build-gha.bat %BUILD_TYPE%
