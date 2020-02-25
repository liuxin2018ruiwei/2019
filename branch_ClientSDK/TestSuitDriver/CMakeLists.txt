cmake_minimum_required (VERSION 2.6)
#project (TransportFaceServiceSDK)

# The version number.
#set (ServiceSDK_VERSION_MAJOR 0)
#set (ServiceSDK_VERSION_MINOR 1)


# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
#  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
#  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_SOURCE_DIR}/src/Include")
include_directories("${PROJECT_SOURCE_DIR}/TestSuitStub")
include_directories("${PROJECT_SOURCE_DIR}/src/CommunicateAgent")
include_directories("${PROJECT_SOURCE_DIR}/TestSuitDriver/TestDriver")
include_directories("${PROJECT_SOURCE_DIR}/TestSuitDriver")
include_directories("${PROJECT_SOURCE_DIR}/TestSuitStub/CommunicateAgent")
include_directories("${PROJECT_SOURCE_DIR}/src/Include")
include_directories("${PROJECT_SOURCE_DIR}/src/Utility/")
include_directories("${PROJECT_SOURCE_DIR}/src/Utility/Observer")
include_directories("${PROJECT_SOURCE_DIR}/src/CommunicateAgent")
include_directories("${PROJECT_SOURCE_DIR}/src/DataCache/")
include_directories("${PROJECT_SOURCE_DIR}/src/FaceDetector/")
include_directories("${PROJECT_SOURCE_DIR}/src/MemoryManage/")
include_directories("${PROJECT_SOURCE_DIR}/src/MsgProcessor/")
include_directories("${PROJECT_SOURCE_DIR}/src/PthreadManage/")
include_directories("${PROJECT_SOURCE_DIR}/src/RequestProcessor/")

aux_source_directory (. CUR_SRC)
aux_source_directory (./TestDriver TESTDRIVER_SRC)
aux_source_directory (../TestSuitStub TESTSUITSTUB_SRC)

add_executable(TestSuitDriver ${CUR_SRC} ${TESTDRIVER_SRC} ${TESTSUITSTUB_SRC})

target_link_libraries( TestSuitDriver DetectFaceSDK)
