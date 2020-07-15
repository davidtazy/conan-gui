message("deploy qt binaries to  ${CMAKE_BINARY_DIR}/qt_deploy")

find_package(PythonInterp REQUIRED)

execute_process(COMMAND 
                  ${PYTHON_EXECUTABLE} deploy.py ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/qt_deploy
                WORKING_DIRECTORY
                  ${CMAKE_CURRENT_LIST_DIR})