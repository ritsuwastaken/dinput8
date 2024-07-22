cmake_minimum_required(VERSION 3.11...3.30)
include(FetchContent)

project(dinput8 VERSION 1.1.0 LANGUAGES CXX)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "This project must be built for 32-bit.")
endif()

if(NOT MSVC)
    message(FATAL_ERROR "This project must be built with MSVC.")
endif()

add_compile_options(
    $<$<CONFIG:>:/MT>
    $<$<CONFIG:Debug>:/MTd>
    $<$<CONFIG:Release>:/MT>
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG master
)
FetchContent_MakeAvailable(minhook)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    LINK_FLAGS "/DEF:${CMAKE_SOURCE_DIR}/src/exports.def"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN DINPUT8_PROJECT_VERSION="${PROJECT_VERSION}")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE minhook psapi)
target_include_directories(${PROJECT_NAME} PRIVATE ${minhook_SOURCE_DIR}/include)
