project(lights C CXX ASM)

# Set compiler options for this project to use the STM32L4
link_libraries(STM32::L4)
# Define the MCU family
add_compile_definitions(STM32L476xx)

# Build a CMSIS library for this board
include(${CMAKE_SOURCE_DIR}/cmake/CMSIS.cmake)

# Use the system file defined in the project sources
target_sources(CMSIS_${PROJECT_NAME}
PRIVATE
  ${CMSIS_DIR}/Device/ST/STM32L4xx/Source/Templates/system_stm32l4xx.c
)

# Include the STM32L4 device family directory for CMSIS
target_include_directories(CMSIS_${PROJECT_NAME}
PUBLIC
  ${CMSIS_DIR}/Device/ST/STM32L4xx/Include
)

# Build a STM32L4 HAL library for this board
include(${CMAKE_SOURCE_DIR}/cmake/STM32L4_HAL.cmake)
# Include the `inc` directory in the HAL library so it can find the config
target_include_directories(HAL_${PROJECT_NAME} PRIVATE inc)

# Add this in once the UMNSVP library is needed or is in devleopment-f1
# Build a UMNSVP library for this board
include(${CMAKE_SOURCE_DIR}/cmake/UMNSVP.cmake)
# Include the `inc` directory in the UMNSVP library so it can find the config
target_include_directories(UMNSVP_${PROJECT_NAME} PRIVATE inc)

# Build a Skylab2 library for this board
include(${CMAKE_SOURCE_DIR}/cmake/Skylab2.cmake)
# Include the `inc` directory in the UMNSVP library so it can find the config
target_include_directories(Skylab2_${PROJECT_NAME} PUBLIC inc)

# target_link_libraries(UMNSVP_${PROJECT_NAME} PUBLIC HAL_${PROJECT_NAME})
# target_link_libraries(UMNSVP_${PROJECT_NAME} PUBLIC CMSIS_${PROJECT_NAME})

# Add the executable for this board
add_executable(${PROJECT_NAME})
# Add the `inc` directory for this board
target_include_directories(${PROJECT_NAME} PRIVATE inc)

# Set all the sources for this board
target_sources(${PROJECT_NAME}
PRIVATE
  src/main.cc
  src/app.cc
  src/stm32l4xx_it.cc
  src/light.cc
  src/timing.cc
  # Use the startup assembly for the L476
  ${STM32L4_STARTUP_DIR}/startup_stm32l476xx.s
)
# When linking steering, link using these libraries
target_link_libraries(${PROJECT_NAME}
PUBLIC
  CMSIS_${PROJECT_NAME}
  HAL_${PROJECT_NAME}
  UMNSVP_${PROJECT_NAME}
  Skylab2_${PROJECT_NAME}
  # Use the STM32L476 Linker Scripts
  -T${STM32L4_DIR}/STM32L476RGTX_FLASH.ld
)