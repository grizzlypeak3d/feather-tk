add_test(
    NAME "Python bellows"
    COMMAND ${Python3_EXECUTABLE} bellows.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python buttons"
    COMMAND ${Python3_EXECUTABLE} buttons.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python dialogs"
    COMMAND ${Python3_EXECUTABLE} dialogs.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python icons"
    COMMAND ${Python3_EXECUTABLE} icons.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python mdi"
    COMMAND ${Python3_EXECUTABLE} mdi.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python simple"
    COMMAND ${Python3_EXECUTABLE} simple.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python sliders"
    COMMAND ${Python3_EXECUTABLE} sliders.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(
    NAME "Python tabs"
    COMMAND ${Python3_EXECUTABLE} tabs.py -exit -log
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
