add_test(
    NAME "Python bellows"
    COMMAND ${Python_EXECUTABLE} bellows.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python buttons"
    COMMAND ${Python_EXECUTABLE} buttons.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python dialogs"
    COMMAND ${Python_EXECUTABLE} dialogs.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python icons"
    COMMAND ${Python_EXECUTABLE} icons.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python panel"
    COMMAND ${Python_EXECUTABLE} panel.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python mdi"
    COMMAND ${Python_EXECUTABLE} mdi.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python simple"
    COMMAND ${Python_EXECUTABLE} simple.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python sliders"
    COMMAND ${Python_EXECUTABLE} sliders.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python tabs"
    COMMAND ${Python_EXECUTABLE} tabs.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python testing"
    COMMAND ${Python_EXECUTABLE} testing.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python textedit"
    COMMAND ${Python_EXECUTABLE} textedit.py -exit -log
        -settingsFile ${CMAKE_CURRENT_BINARY_DIR}/textedit-test.json
        -logFile ${CMAKE_CURRENT_BINARY_DIR}/textedit-test.log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
