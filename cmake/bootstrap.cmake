# Run this as cmake -P bootstrap.cmake from buildPrompt.bat
set(PROJECT_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build/")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bld")
execute_process(
    COMMAND ${CMAKE_COMMAND} -G Ninja -B bld
    RESULT_VARIABLE RESULT
)
if(NOT RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to configure build: ${RESULT}")
endif()

execute_process(
    COMMAND ${CMAKE_COMMAND} --build bld --target download-all
    RESULT_VARIABLE RESULT
)
if(NOT RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to download dependencies: ${RESULT}")
endif()

execute_process(
    COMMAND ${CMAKE_COMMAND} --build bld --verbose
    RESULT_VARIABLE SUPERBUILD_RESULT
)
if(NOT SUPERBUILD_RESULT EQUAL 0)
    message(FATAL_ERROR "Build failed with code: ${SUPERBUILD_RESULT}")
endif()

file(TOUCH "${PROJECT_BUILD_DIR}/build_complete.marker")
message(STATUS "SuperBuild completed successfully...")
