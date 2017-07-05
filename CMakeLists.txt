cmake_minimum_required(VERSION 3.0)
project(license)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

set(SOURCE_FILES  apps.c CustomOptionDescription.cpp OptionPrinter.cpp CSignature.cpp CLicense.cpp CCertificate.cpp CLicTool.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
# link_directories("/usr/local/ssl/lib")

add_library(licchk STATIC ${SOURCE_FILES})

set(PROJECT_GENLIC genlic)
add_executable(${PROJECT_GENLIC} ${SOURCE_FILES} genlic.cpp)
target_link_libraries(${PROJECT_GENLIC} ssl crypto dl boost_system boost_filesystem boost_program_options boost_regex log4cplus)

set(PROJECT_SIGFILE sigfile)
add_executable(${PROJECT_SIGFILE} ${SOURCE_FILES} sigfile.cpp)
target_link_libraries(${PROJECT_SIGFILE} ssl crypto dl boost_system boost_filesystem boost_program_options boost_regex log4cplus)

set(PROJECT_MYTEST mytest)
add_executable(${PROJECT_MYTEST} ${SOURCE_FILES} test.cpp)
target_link_libraries(${PROJECT_MYTEST} ssl crypto dl boost_system boost_filesystem boost_program_options boost_regex log4cplus)
