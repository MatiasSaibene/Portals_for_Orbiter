cmake_minimum_required(VERSION 3.16)
project(PortalsForOrbiter)

set(CMAKE_CXX_STANDARD 20)

set(ORBITER_ROOT "$ENV{HOME}/orbiter_test/Orbiter")
set(ORBITER_SDK "${ORBITER_ROOT}/Orbitersdk/")
set(ORBITER_SDK_INCLUDE "${ORBITER_SDK}/Orbitersdk/include")
set(LIBRARY_DIRECTORY "${ORBITER_ROOT}/Orbitersdk/lib")

# Incluir todas las bibliotecas estáticas de Orbiter
file(GLOB LIBRARIES "${LIBRARY_DIRECTORY}/*.a")

# Fuentes y encabezados para BluePortal
set(BLUE_SOURCES blueportal.cpp)
set(BLUE_HEADERS blueportal.h)

# Fuentes y encabezados para OrangePortal
set(ORANGE_SOURCES orangeportal.cpp)
set(ORANGE_HEADERS orangeportal.h)

# Biblioteca BluePortal
add_library(BluePortal SHARED ${BLUE_SOURCES} ${BLUE_HEADERS})
target_include_directories(BluePortal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${ORBITER_SDK_INCLUDE})
target_link_libraries(BluePortal PRIVATE ${LIBRARIES})
target_compile_options(BluePortal PRIVATE -fPIC)

# Biblioteca OrangePortal
add_library(OrangePortal SHARED ${ORANGE_SOURCES} ${ORANGE_HEADERS})
target_include_directories(OrangePortal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${ORBITER_SDK_INCLUDE})
target_link_libraries(OrangePortal PRIVATE ${LIBRARIES})
target_compile_options(OrangePortal PRIVATE -fPIC)
