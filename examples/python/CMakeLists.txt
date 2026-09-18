# Running each example is the test: an example is a program, and one that
# names a binding that does not exist fails only when it is run. Nothing
# else covers them -- the Python unit tests are their own files under
# tests/ -- so without this an example can be broken in the repository and
# every job stays green.
#
# Every one of them opens a window, so there is nothing to run where there
# is no OpenGL. They are still read by CMake, which is no check at all;
# that is the trade.
if(ftk_TESTS_NO_GL)
    return()
endif()

# "-exit" runs one pass of the event loop and quits, which is enough to
# build the interface, lay it out and draw it once. "-log" puts what the
# application logged on stderr, so a failure says why.
set(ftk_PYTHON_EXAMPLES
    bellows
    buttons
    dialogs
    icons
    mdi
    panel
    simple
    sliders
    tabs)
foreach(ftk_PYTHON_EXAMPLE ${ftk_PYTHON_EXAMPLES})
    add_test(
        NAME "Python ${ftk_PYTHON_EXAMPLE}"
        COMMAND ${Python_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/run-example.py
            ${CMAKE_INSTALL_PREFIX}/bin
            ${ftk_PYTHON_EXAMPLE}.py -exit -log
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

# Apart from two. The text editor writes settings and a log, which belong
# in the build directory rather than beside the source; and the testing
# example drives itself and exits on its own, so "-exit" would stop it
# before it had done anything.
add_test(
    NAME "Python textedit"
    COMMAND ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/run-example.py
        ${CMAKE_INSTALL_PREFIX}/bin
        textedit.py -exit -log
        -settingsFile ${CMAKE_CURRENT_BINARY_DIR}/textedit-test.json
        -logFile ${CMAKE_CURRENT_BINARY_DIR}/textedit-test.log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND ftk_PYTHON_EXAMPLES textedit)

add_test(
    NAME "Python testing"
    COMMAND ${Python_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/run-example.py
        ${CMAKE_INSTALL_PREFIX}/bin
        testing.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND ftk_PYTHON_EXAMPLES testing)

# The just-built package ahead of any external PYTHONPATH, the same as the
# Python tests, so a plain ctest run works anywhere.
foreach(ftk_PYTHON_EXAMPLE ${ftk_PYTHON_EXAMPLES})
    set_tests_properties("Python ${ftk_PYTHON_EXAMPLE}" PROPERTIES
        ENVIRONMENT_MODIFICATION
            "PYTHONPATH=path_list_prepend:${CMAKE_BINARY_DIR}/python")
endforeach()
