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

cmake ^
    -S %SOURCE_DIR%/etc/SuperBuild ^
    -B sb-%BUILD_TYPE% ^
    -C %CONFIG_FILE% ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX=%CD%/install-%BUILD_TYPE% ^
    -DCMAKE_PREFIX_PATH=%CD%/install-%BUILD_TYPE%
cmake --build sb-%BUILD_TYPE% --config %BUILD_TYPE%

cmake ^
    -S %SOURCE_DIR% ^
    -B build-%BUILD_TYPE% ^
    -C %CONFIG_FILE% ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX=%CD%/install-%BUILD_TYPE% ^
    -DCMAKE_PREFIX_PATH=%CD%/install-%BUILD_TYPE%
cmake --build build-%BUILD_TYPE% --config %BUILD_TYPE%
cmake --build build-%BUILD_TYPE% --config %BUILD_TYPE% --target install
