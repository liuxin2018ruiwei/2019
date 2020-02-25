cmake_minimum_required (VERSION 2.6)
#project (TransportFaceServiceSDK)

# The version number.
#set (DetectFaceSDK_VERSION_MAJOR 0)
#set (DetectFaceSDK_VERSION_MINOR 1)

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

include_directories("${PROJECT_SOURCE_DIR}/src/FaceDetector/CountFace")
include_directories("${PROJECT_SOURCE_DIR}/src/MemoryManage/")
include_directories("${PROJECT_SOURCE_DIR}/src/MsgProcessor/")
include_directories("${PROJECT_SOURCE_DIR}/src/MsgProcessor/HttpMsgProcessor")
include_directories("${PROJECT_SOURCE_DIR}/src/MsgProcessor/SDKMsgProcessor")
include_directories("${PROJECT_SOURCE_DIR}/src/MsgProcessor/UartMsgProcessor")
include_directories("${PROJECT_SOURCE_DIR}/src/PthreadManage/")
include_directories("${PROJECT_SOURCE_DIR}/src/RequestProcessor/")
include_directories("${PROJECT_SOURCE_DIR}/src/DetectFaceIF/")

include_directories("${PROJECT_SOURCE_DIR}/TestSuitDriver/TestDriver")
include_directories("${PROJECT_SOURCE_DIR}/TestSuitDriver")
include_directories("${PROJECT_SOURCE_DIR}/TestSuitStub")
include_directories("${PROJECT_SOURCE_DIR}/TestSuitStub/CommunicateAgent")
include_directories("${PROJECT_SOURCE_DIR}/src/FaceDetector/Zmode")

if("${X86_COMPLIE_SET}")
aux_source_directory (. CUR_SRC)
aux_source_directory (./DetectFaceIF  DETECTFACEIF_SRC)
aux_source_directory (./Utility  UTILITY_SRC)
aux_source_directory (./Utility/Observer  OBSERVER_SRC)
aux_source_directory (./CommunicateAgent  COMMUNICATEAGENT_SRC)
aux_source_directory (./DataCache  DATACACHE_SRC)
aux_source_directory (./FaceDetector  FACEDETECTOR_SRC)
aux_source_directory (./FaceDetector/CountFace  COUNTFACE_SRC)


aux_source_directory (./MemoryManage MEMORYMANAGE_SRC)
aux_source_directory (./MsgProcessor MSGPROCESSOR_SRC)
aux_source_directory (./MsgProcessor/HttpMsgProcessor HTTPMSGPROCESSOR_SRC)
aux_source_directory (./MsgProcessor/SDKMsgProcessor SDKMSGPROCESSOR_SRC)
aux_source_directory (./MsgProcessor/UartMsgProcessor UARTMSGPROCESSOR_SRC)
aux_source_directory (./PthreadManage PTHREADMANAGE_SRC)
aux_source_directory (./RequestProcessor REQUESTPROCESSOR_SRC)
aux_source_directory (./FaceDetector/Zmode  ZMODE_SRC)

aux_source_directory ( ${PROJECT_SOURCE_DIR}/TestSuitStub/CommunicateAgent STUBCOMMUNICATE_SRC)



add_library(DetectFaceSDK SHARED ${CUR_SRC} ${DETECTFACEIF_SRC} 
${UTILITY_SRC} ${OBSERVER_SRC} ${COMMUNICATEAGENT_SRC} ${DATACACHE_SRC} 
${FACEDETECTOR_SRC} ${COUNTFACE_SRC} ${MEMORYMANAGE_SRC} ${MSGPROCESSOR_SRC} ${PTHREADMANAGE_SRC} 
${REQUESTPROCESSOR_SRC} ${HTTPMSGPROCESSOR_SRC} ${SDKMSGPROCESSOR_SRC}
${UARTMSGPROCESSOR_SRC} ${STUBCOMMUNICATE_SRC}  ${ZMODE_SRC} )
target_link_libraries( DetectFaceSDK pthread)
target_link_libraries( DetectFaceSDK rt)

else()

include_directories("${PROJECT_SOURCE_DIR}/src/FaceDetector/HaiSi/HaisiEcode")
include_directories("${PROJECT_SOURCE_DIR}/src/FaceDetector/HaiSi/include")



include_directories("${PROJECT_SOURCE_DIR}/lib")

aux_source_directory (. CUR_SRC)
aux_source_directory (./DetectFaceIF  DETECTFACEIF_SRC)
aux_source_directory (./Utility  UTILITY_SRC)
aux_source_directory (./Utility/Observer  OBSERVER_SRC)
aux_source_directory (./CommunicateAgent  COMMUNICATEAGENT_SRC)
aux_source_directory (./DataCache  DATACACHE_SRC)
aux_source_directory (./FaceDetector  FACEDETECTOR_SRC)
aux_source_directory (./FaceDetector/CountFace  COUNTFACE_SRC)
aux_source_directory (./FaceDetector/HaiSi/HaisiEcode  HAISI_ECODE_SRC)
aux_source_directory (./FaceDetector/Zmode  ZMODE_SRC)

aux_source_directory (./MemoryManage MEMORYMANAGE_SRC)
aux_source_directory (./MsgProcessor MSGPROCESSOR_SRC)
aux_source_directory (./MsgProcessor/HttpMsgProcessor HTTPMSGPROCESSOR_SRC)
aux_source_directory (./MsgProcessor/SDKMsgProcessor SDKMSGPROCESSOR_SRC)
aux_source_directory (./MsgProcessor/UartMsgProcessor UARTMSGPROCESSOR_SRC)
aux_source_directory (./PthreadManage PTHREADMANAGE_SRC)
aux_source_directory (./RequestProcessor REQUESTPROCESSOR_SRC)

aux_source_directory ( ${PROJECT_SOURCE_DIR}/TestSuitStub/CommunicateAgent STUBCOMMUNICATE_SRC)



add_library(DetectFaceSDK SHARED ${CUR_SRC} ${DETECTFACEIF_SRC} 
${UTILITY_SRC} ${OBSERVER_SRC} ${COMMUNICATEAGENT_SRC} ${DATACACHE_SRC} 
${FACEDETECTOR_SRC} ${COUNTFACE_SRC} ${HAISI_ECODE_SRC} ${ZMODE_SRC}
${MEMORYMANAGE_SRC} ${MSGPROCESSOR_SRC} ${PTHREADMANAGE_SRC} 
${REQUESTPROCESSOR_SRC} ${HTTPMSGPROCESSOR_SRC} ${SDKMSGPROCESSOR_SRC}
${UARTMSGPROCESSOR_SRC} ${STUBCOMMUNICATE_SRC} )
target_link_libraries( DetectFaceSDK pthread)
target_link_libraries( DetectFaceSDK rt)
add_library(libmpiTest STATIC IMPORTED)  
set_target_properties(libmpiTest    PROPERTIES IMPORTED_LOCATION  "${PROJECT_SOURCE_DIR}/lib/libmpi.a" )
target_link_libraries(DetectFaceSDK libmpiTest)

add_library(libupvqeTest STATIC IMPORTED)  
set_target_properties(libupvqeTest    PROPERTIES IMPORTED_LOCATION  "${PROJECT_SOURCE_DIR}/lib/libupvqe.a" )
target_link_libraries(DetectFaceSDK libupvqeTest)

add_library(libdnvqeTest STATIC IMPORTED)  
set_target_properties(libdnvqeTest    PROPERTIES IMPORTED_LOCATION  "${PROJECT_SOURCE_DIR}/lib/libdnvqe.a" )
target_link_libraries(DetectFaceSDK libdnvqeTest)

add_library(libVoiceEngineTest STATIC IMPORTED)  
set_target_properties(libVoiceEngineTest    PROPERTIES IMPORTED_LOCATION  "${PROJECT_SOURCE_DIR}/lib/libVoiceEngine.a" )
target_link_libraries(DetectFaceSDK libVoiceEngineTest)

add_library(libispTest STATIC IMPORTED)  
set_target_properties(libispTest    PROPERTIES IMPORTED_LOCATION  "${PROJECT_SOURCE_DIR}/lib/libisp.a" )
target_link_libraries(DetectFaceSDK libispTest)

add_library(lib_hidefogTest STATIC IMPORTED)  
set_target_properties(lib_hidefogTest    PROPERTIES IMPORTED_LOCATION  "${PROJECT_SOURCE_DIR}/lib/lib_hidefog.a" )
target_link_libraries(DetectFaceSDK lib_hidefogTest)
endif()
