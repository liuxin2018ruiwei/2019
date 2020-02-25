cmake_minimum_required (VERSION 2.6)

include_directories("${PROJECT_SOURCE_DIR}/src/Include")
#include_directories("${PROJECT_SOURCE_DIR}/TestSuitDriver")

aux_source_directory (. CUR_SRC)
#aux_source_directory (./TestDriver TESTDRIVER_SRC)


add_executable(Sample ${CUR_SRC} )

target_link_libraries( Sample DetectFaceSDK)
