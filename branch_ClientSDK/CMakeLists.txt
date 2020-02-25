cmake_minimum_required (VERSION 2.6)
project (DetectSDK)

# The version number.
set (DetectSDK_VERSION_MAJOR 0)
set (DetectSDK_VERSION_MINOR 1)
set (EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")
set (LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")

# specify the cross compiler
#SET(CMAKE_C_COMPILER /opt/hisi-linux/x86-arm/arm-hisiv400-linux/target/bin/arm-hisiv400-linux-gcc)
SET(CMAKE_C_COMPILER  /opt/hisi-linux/x86-arm/arm-hisiv300-linux/target/bin/arm-hisiv300-linux-gcc)

#SET(CMAKE_CXX_COMPILER /opt/hisi-linux/x86-arm/arm-hisiv400-linux/target/bin/arm-hisiv400-linux-g++)
SET(CMAKE_CXX_COMPILER  /opt/hisi-linux/x86-arm/arm-hisiv300-linux/target/bin/arm-hisiv300-linux-g++)

#link_directories( /opt/hisi-linux/x86-arm/arm-hisiv400-linux/arm-hisiv400-linux-gnueabi/lib /opt/hisi-linux/x86-arm/arm-hisiv400-linux/target/usr/lib )
link_directories(/opt/hisi-linux/x86-arm/arm-hisiv300-linux/arm-hisiv300-linux-uclibcgnueabi/lib  /opt/hisi-linux/x86-arm/arm-hisiv300-linux/target/usr/lib )

add_compile_options(-std=c++11)
add_subdirectory(src)
add_subdirectory(TestSuitDriver)



# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
#  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
#  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
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



#add_library(TransportFaceServiceSDK SHARED ${CUR_SRC} ${TRANSPORTFACEIF_SRC} )

#add_executable(TestSuitDriver ${CUR_SRC} ${TRANSPORTFACEIF_SRC} ${TESTSUITDRIVER_SRC} )
#target_link_libraries( TestSuitDriver TransportFaceServiceSDK)
