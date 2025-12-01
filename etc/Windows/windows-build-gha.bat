set BUILD_TYPE=%1

cmake ^
    -S feather-tk/etc/SuperBuild ^
    -B sbuild-%BUILD_TYPE% ^
    -Dftk_API=%FTK_API% ^
    -Dftk_nfd=%FTK_NFD% ^
    -Dftk_PYTHON=%FTK_PYTHON% ^
    -DBUILD_SHARED_LIBS=%BUILD_SHARED_LIBS% ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX=%CD%/install-%BUILD_TYPE% ^
    -DCMAKE_PREFIX_PATH=%CD%/install-%BUILD_TYPE%
cmake --build sbuild-%BUILD_TYPE% -j %JOBS% --config %BUILD_TYPE%

cmake ^
    -S feather-tk ^
    -B build-%BUILD_TYPE% ^
    -Dftk_API=%FTK_API% ^
    -Dftk_nfd=%FTK_NFD% ^
    -Dftk_PYTHON=%FTK_PYTHON% ^
    -Dftk_TESTS=%FTK_TESTS% ^
    -Dftk_EXAMPLES=%FTK_EXAMPLES% ^
    -Dftk_GCOV=%FTK_GCOV% ^
    -DBUILD_SHARED_LIBS=%BUILD_SHARED_LIBS% ^
    -DCMAKE_INSTALL_PREFIX=%CD%/install-%BUILD_TYPE% ^
    -DCMAKE_PREFIX_PATH=%CD%/install-%BUILD_TYPE% ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
cmake --build build-%BUILD_TYPE% -j %JOBS% --config %BUILD_TYPE%
cmake --build build-%BUILD_TYPE% --config %BUILD_TYPE% --target install
