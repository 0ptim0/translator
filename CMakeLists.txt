cmake_minimum_required(VERSION 3.15)

set(PROJ_NAME "translator")
set(MCU_FAMILY stm32f4)
set(MCU_MODEL stm32f401re)

execute_process (
    COMMAND bash -c "git show --quiet | grep 'commit ' | sed 's/commit //' | awk '{print substr($0,1,8)}' | tr '\n' ' '"
    OUTPUT_VARIABLE GIT_HASH
)

execute_process (
    COMMAND bash -c "if [ -z \"$(git status -s)\" ]; then echo clean; else echo dirty; fi | tr '\n' ' '"
    OUTPUT_VARIABLE GIT_STATE
)

set(CMAKE_TOOLCHAIN_FILE bsp/mcu/${MCU_FAMILY}/${MCU_FAMILY}_toolchain.cmake)
set(MCU_MAP_CREATION "ON")

project(${PROJ_NAME} C CXX ASM)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/firmware)

include_directories(
    ${BSP}
)

add_subdirectory(bsp)
add_definitions(-DGIT_HASH=${GIT_HASH})
add_definitions(-DGIT_STATE=${GIT_STATE})

message("\n${PROJECT_NAME}:")
message("   MCU family: ${MCU_FAMILY}")
message("   MCU model: ${MCU_MODEL}")
message("   Toolchain: ${CMAKE_TOOLCHAIN_FILE}")
message("   C Compiler: ${CMAKE_C_COMPILER}")
message("   C++ Compiler: ${CMAKE_CXX_COMPILER}")
message("   Compiler flags: ${COMPILER_FLAGS}")
message("   Linker flags: ${LINKER_FLAGS}")
message("   Version: ${GIT_HASH} ${GIT_STATEGIT_STATE}")

add_executable(${PROJECT_NAME}.elf
    main.cpp
)

target_link_libraries(${PROJECT_NAME}.elf PUBLIC
    mcu-static
    -lc -lm -lnosys
)

set(ELF_FILE ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.elf)
set(HEX_FILE ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.hex)
set(BIN_FILE ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin)

add_custom_command(TARGET "${PROJECT_NAME}.elf" POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Oihex  ${ELF_FILE} ${HEX_FILE}
)

add_custom_command(TARGET "${PROJECT_NAME}.elf" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${HEX_FILE} "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.hex"
    COMMAND ${CMAKE_COMMAND} -E copy ${BIN_FILE} "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin"
    COMMAND ${CMAKE_SIZE} -A ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.elf
    COMMAND ${CMAKE_SIZE} --format=berkeley ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.hex
)
