message("deploy qt binaries to  ${CMAKE_INSTALL_PREFIX}")

find_package(PythonInterp REQUIRED)

execute_process(COMMAND 
                  ${PYTHON_EXECUTABLE} deploy.py ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}
                WORKING_DIRECTORY
                  ${CMAKE_CURRENT_LIST_DIR})