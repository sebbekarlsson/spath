cmake_minimum_required(VERSION 3.20)

project(spath)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/)
set(COMMANDS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)

if (EXISTS ${COMMANDS_FILE})
  file(COPY ${COMMANDS_FILE} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB sourcefiles ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(spath_e ${sourcefiles})
add_library(spath_shared SHARED ${sourcefiles})
add_library(spath_static STATIC ${sourcefiles})

target_compile_options(spath_shared PUBLIC -fPIC)


target_include_directories(
  spath_static
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


 target_include_directories(
  spath_shared
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)
