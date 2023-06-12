cmake_minimum_required(VERSION 3.0)

project(bygone)

# Set project layout variables
set(BYGONE_PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(BYGONE_LIB_DIR ${BYGONE_PROJECT_ROOT}/build/lib)
set(BYGONE_BIN_DIR ${BYGONE_PROJECT_ROOT}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BYGONE_PROJECT_ROOT}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BYGONE_PROJECT_ROOT}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BYGONE_PROJECT_ROOT}/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BYGONE_LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BYGONE_LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BYGONE_LIB_DIR})

# depenencies
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_HOME})

# set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug
        CACHE
        STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# compilation flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# definitions
add_definitions("-std=c++14 -W -Wall -Wextra -Wfatal-errors -Werror") #
add_definitions("-Wno-unused-parameter -Wno-unused-local-typedefs -Wno-missing-field-initializers")
add_definitions("-shared-libgcc -libstdc++")

add_definitions(
    -DFUSION_MAX_VECTOR_SIZE=30 
    -DBOOST_SPIRIT_USE_PHOENIX_V3
    -DBOOST_SPIRIT_UNICODE
    -DBOOST_SPIRIT_NO_PREDEFINED_TERMINALS
    -DBOOST_LOG_WITHOUT_WCHAR_T
    -DBOOST_SPIRIT_UNICODE
    -DBOOST_ALL_DYN_LINK)

# modules
add_subdirectory(foundation)
add_subdirectory(kernel)
add_subdirectory(ui)
add_subdirectory(launcher)
