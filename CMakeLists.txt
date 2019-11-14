project(miniexample)

cmake_minimum_required(VERSION 2.8)

set(INCLUDE_DIR "./include")
set(SRC_DIR "./src")
get_filename_component(SRC_DIR "./src" ABSOLUTE)
set(ORKAGD_DIR "${SRC_DIR}/3rdparty/fpgaInfrastructure/sw/OrkaGenericDriver/src/")
set(ORKAGD_INTERPRETER_DIR "${SRC_DIR}")
set(TARGET_BOARD "" CACHE STRING "Specify the Target FPGA Board (arty, arty-a7-100, vcu118)")

# Hostprogram
include_directories(${ORKAGD_DIR})
include_directories(${INCLUDE_DIR})

add_definitions(-DJSON_DIR="${CMAKE_CURRENT_BINARY_DIR}/bitstreams")
add_definitions(-DJSON_NAME="3fd3a845-59f5-4393-adf6-5f6551fba982.json")
add_definitions(-DORKAGD_INTERPRETER_DIR="${ORKAGD_INTERPRETER_DIR}")
set(HOSTPROGRAM_DIR "${SRC_DIR}/hostprogram")
set(HOSTPROGRAM_SOURCES
    ${HOSTPROGRAM_DIR}/hostprogram.cpp
)
# Build liborkagd.so with provided makefile
add_custom_target(
    liborkagd
    ALL
    COMMAND make liborkagd.so
    WORKING_DIRECTORY ${ORKAGD_DIR}
)
add_executable(hostprogram ${HOSTPROGRAM_SOURCES})
target_link_libraries(
    hostprogram
    "${ORKAGD_DIR}/liborkagd.so"
)
add_dependencies(hostprogram liborkagd)

#-------------------------------------------------------------------------------
#infrastructure/bitstream
add_custom_target(
    infrastructure
    COMMAND ${SRC_DIR}/build_infrastructure ${CMAKE_CURRENT_BINARY_DIR} ${TARGET_BOARD}
)

add_custom_target(
    build_hardware
    ALL
    DEPENDS ips infrastructure
)

#-------------------------------------------------------------------------------
#firstip
add_custom_target(
    firstip
    COMMAND ${SRC_DIR}/build_ip firstip ${CMAKE_CURRENT_BINARY_DIR} ${TARGET_BOARD}
    COMMAND unzip -o ${CMAKE_CURRENT_BINARY_DIR}/2018.2/repo/firstip/hellmann_miniexample_firstip_0_1_10ns.zip -d ${CMAKE_CURRENT_BINARY_DIR}/2018.2/repo/firstip/
)

#secondip
add_custom_target(
    secondip
    COMMAND ${SRC_DIR}/build_ip secondip ${CMAKE_CURRENT_BINARY_DIR} ${TARGET_BOARD}
    COMMAND unzip -o ${CMAKE_CURRENT_BINARY_DIR}/2018.2/repo/secondip/hellmann_miniexample_secondip_0_1_10ns.zip -d ${CMAKE_CURRENT_BINARY_DIR}/2018.2/repo/secondip/
)

add_custom_target(
    ips
    ALL
    DEPENDS firstip secondip
)



