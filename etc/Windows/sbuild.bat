rem Build the dependencies and then feather-tk, into directories beside the
rem current one. What to build is in etc/Config/*.cmake rather than here: this
rem script is only the part that differs between platforms.

set SOURCE_DIR=%1
set BUILD_TYPE=%2
set CONFIG=%3
IF "%CONFIG%"=="" set CONFIG=default
set CONFIG_FILE=%SOURCE_DIR%/etc/Config/%CONFIG%.cmake

rem Build with every core unless told otherwise; cmake --build reads this.
IF "%CMAKE_BUILD_PARALLEL_LEVEL%"=="" set CMAKE_BUILD_PARALLEL_LEVEL=%NUMBER_OF_PROCESSORS%

rem SBUILD_STAGES picks the stages: "deps" builds only the dependencies, into
rem install-<type>, and "project" only feather-tk, against dependencies
rem already there. Unset, both. CI caches the dependencies' install directory
rem between the two, which is all the project needs of them.
if "%SBUILD_STAGES%"=="project" goto project

cmake ^
    -S %SOURCE_DIR%/etc/SuperBuild ^
    -B sb-%BUILD_TYPE% ^
    -C %CONFIG_FILE% ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX=%CD%/install-%BUILD_TYPE% ^
    -DCMAKE_PREFIX_PATH=%CD%/install-%BUILD_TYPE%
cmake --build sb-%BUILD_TYPE% --config %BUILD_TYPE%
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

if "%SBUILD_STAGES%"=="deps" exit /b 0

:project

cmake ^
    -S %SOURCE_DIR% ^
    -B build-%BUILD_TYPE% ^
    -C %CONFIG_FILE% ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX=%CD%/install-%BUILD_TYPE% ^
    -DCMAKE_PREFIX_PATH=%CD%/install-%BUILD_TYPE%
cmake --build build-%BUILD_TYPE% --config %BUILD_TYPE%
cmake --build build-%BUILD_TYPE% --config %BUILD_TYPE% --target install
