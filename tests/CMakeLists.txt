add_subdirectory(ftktest)

if(ftk_PYTHON)
    add_test(
        NAME Python
        COMMAND ${Python3_EXECUTABLE} -m unittest discover -v ${CMAKE_CURRENT_SOURCE_DIR})
endif()
