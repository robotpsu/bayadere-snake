cmake_minimum_required(VERSION 2.8.4)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../../cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME bayadere_snake_ino)
set(${PROJECT_NAME}_SKETCH snake/snake.ino)
set(${PROJECT_NAME}_ARDLIBS FlexActuator)
set(${PROJECT_NAME}_BOARD mega)
set(ARDUINO_CPU atmega2560)

project(${PROJECT_NAME})
generate_arduino_firmware(${PROJECT_NAME})
