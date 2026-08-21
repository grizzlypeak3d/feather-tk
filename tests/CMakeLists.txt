add_subdirectory(ftk-test)

if(ftk_PYTHON)
    add_test(
        NAME ftkPy-test
        COMMAND ${Python3_EXECUTABLE} -m unittest discover -v ${CMAKE_CURRENT_SOURCE_DIR})
    # The just-built module rather than an external PYTHONPATH, so a plain
    # ctest run works anywhere. The flag crosses to Python as an environment
    # variable, where the tests that need OpenGL read it and skip.
    set(PYTHON_TEST_ENV "PYTHONPATH=path_list_append:$<TARGET_FILE_DIR:ftkPy>")
    if(ftk_TESTS_NO_GL)
        list(APPEND PYTHON_TEST_ENV "FTK_TESTS_NO_GL=set:1")
    endif()
    # A hang should fail rather than run until the job is killed, the same as
    # the C++ suite next door. This one runs in seconds, so 900 only ever
    # fires on a hang.
    set_tests_properties(ftkPy-test PROPERTIES
        ENVIRONMENT_MODIFICATION "${PYTHON_TEST_ENV}"
        TIMEOUT 900)
endif()
