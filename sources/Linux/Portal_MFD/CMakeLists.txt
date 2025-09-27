cmake_minimum_required(VERSION 3.10)

project(PortalMFD LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ORBITER_ROOT "$ENV{HOME}/orbiter_test/Orbiter")
set(ORBITER_SDK "${ORBITER_ROOT}/Orbitersdk")
set(ORBITER_SDK_INCLUDE "${ORBITER_SDK}/Orbitersdk/include")

set(SOURCES
    main.cpp
)
set(HEADERS
    main.hpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})


add_compile_options(-fPICg)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${ORBITER_SDK}/lib/libOrbitersdk.a
)
