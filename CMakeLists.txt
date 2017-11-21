CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

# For sanitizers
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

PROJECT(pbrt)

ENABLE_TESTING()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
            "MinSizeRel" "RelWithDebInfo")
endif ()

if (NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/ext/openexr/OpenEXR")
    message(FATAL_ERROR "The OpenEXR submodule directory is missing! "
            "You probably did not clone the project with --recursive. It is possible to recover "
            "by running \"git submodule update --init --recursive\"")
endif ()

if (NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/ext/glog")
    message(FATAL_ERROR "The glog submodule directory is missing! "
            "You probably did not clone the project with --recursive, or first checked out "
            "pbrt before it was added. It is possible to recover by running "
            "\"git submodule update --init --recursive\"")
endif ()

IF (CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null")
ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")
ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    FIND_PROGRAM(XIAR xiar)
    IF (XIAR)
        SET(CMAKE_AR "${XIAR}")
    ENDIF (XIAR)
    MARK_AS_ADVANCED(XIAR)

    FIND_PROGRAM(XILD xild)
    IF (XILD)
        SET(CMAKE_LINKER "${XILD}")
    ENDIF (XILD)
    MARK_AS_ADVANCED(XILD)

    # ICC will default to -fp-model fast=1, which performs value-unsafe optimizations which will
    # cause pbrt_test to fail. For safety, -fp-model precise is explicitly set here by default.
    set(FP_MODEL "precise" CACHE STRING "The floating point model to compile with.")
    set_property(CACHE FP_MODEL PROPERTY STRINGS "precise" "fast=1" "fast=2")

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fp-model ${FP_MODEL}")
ENDIF ()

IF (MSVC)
    ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(/D YY_NO_UNISTD_H)
ENDIF ()

FIND_PACKAGE(Sanitizers)
FIND_PACKAGE(Threads)

IF (CMAKE_BUILD_TYPE MATCHES RELEASE)
    ADD_DEFINITIONS(-DNDEBUG)
ENDIF ()

# Optionally use Bison and Flex to regenerate parser files
# Use pregenerated files otherwise (may be outdated)
# todo: enable these codes.
SET(PBRT_YACC_LEX_SOURCE)
#FIND_PACKAGE(BISON)
#FIND_PACKAGE(FLEX)
#IF (BISON_FOUND AND FLEX_FOUND)
#    SET(BisonOutput ${CMAKE_BINARY_DIR}/pbrtparse.cpp)
#    ADD_CUSTOM_COMMAND(
#            OUTPUT ${BisonOutput} ${CMAKE_BINARY_DIR}/pbrtparse.h
#            DEPENDS ${CMAKE_SOURCE_DIR}/src/parse/pbrtparse.y
#            COMMAND ${BISON_EXECUTABLE} -d -v -t
#            --output=${CMAKE_BINARY_DIR}/pbrtparse.c
#            ${CMAKE_SOURCE_DIR}/src/parse/pbrtparse.y
#            COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/pbrtparse.c ${CMAKE_BINARY_DIR}/pbrtparse.cpp
#            COMMENT "Generating pbrtparse.cpp and pbrtparse.h"
#    )
#
#    SET(FlexOutput ${CMAKE_BINARY_DIR}/pbrtlex.cpp)
#    ADD_CUSTOM_COMMAND(
#            OUTPUT ${FlexOutput}
#            DEPENDS ${CMAKE_BINARY_DIR}/pbrtparse.h ${CMAKE_SOURCE_DIR}/src/parse/pbrtlex.ll
#            COMMAND ${FLEX_EXECUTABLE}
#            -o${FlexOutput}
#            ${CMAKE_SOURCE_DIR}/src/parse/pbrtlex.ll
#            COMMENT "Generating pbrtlex.cpp"
#    )
#
#    SET(PBRT_YACC_LEX_SOURCE
#            ${BisonOutput}
#            ${FlexOutput}
#            )
#ELSE ()
#    SET(PBRT_YACC_LEX_SOURCE
#            src/parse/pbrtlex.cpp
#            src/parse/pbrtparse.cpp
#            )
#ENDIF ()


IF (WIN32)
    # Build zlib (only on Windows)
    SET(ZLIB_BUILD_STATIC_LIBS ON CACHE BOOL " " FORCE)
    SET(ZLIB_BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
    ADD_SUBDIRECTORY(src/ext/zlib)

    SET(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/ext/zlib")
    SET(ZLIB_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/src/ext/zlib/$<CONFIGURATION>/zlibstatic.lib")
    SET_PROPERTY(TARGET zlibstatic PROPERTY FOLDER "dependencies")
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/src/ext/zlib")
ENDIF ()

SET(ILMBASE_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
SET(ILMBASE_BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
ADD_SUBDIRECTORY(src/ext/openexr)
SET_PROPERTY(TARGET IexMath eLut toFloat b44ExpLogTable dwaLookups CopyIlmBaseLibs IlmThread Half Iex Imath IlmImf PROPERTY FOLDER "dependencies")
INCLUDE_DIRECTORIES(
        src/ext/openexr/IlmBase/Imath
        src/ext/openexr/IlmBase/Half
        src/ext/openexr/IlmBase/Iex
        src/ext/openexr/OpenEXR/IlmImf
        ${CMAKE_BINARY_DIR}/src/ext/openexr/IlmBase/config
        ${CMAKE_BINARY_DIR}/src/ext/openexr/OpenEXR/config
)

IF (WIN32)
    SET(OPENEXR_LIBS IlmImf Imath Half zlibstatic)
ELSE ()
    SET(OPENEXR_LIBS IlmImf Imath Half)
ENDIF ()

SET(WITH_GFLAGS OFF CACHE BOOL "Use gflags")
SET(BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
ADD_SUBDIRECTORY(src/ext/glog)
SET_PROPERTY(TARGET glog PROPERTY FOLDER "dependencies")
INCLUDE_DIRECTORIES(
        src/ext/glog/src
        ${CMAKE_BINARY_DIR}/src/ext/glog
)


# Set the source files
# todo: complete source list

SET(PBRT_CORE_HEADERS
        src/core/pbrt.h
        src/core/port.h
        src/core/stringprint.h
        src/core/geometry.h
        src/core/transform.h
        src/core/quaternion.h
        src/core/error.h
        src/core/api.h
        src/core/interaction.h
        src/core/shape.h
        src/core/efloat.h
        src/core/memory.h
        src/core/primitives.h
        src/core/material.h
        src/core/zztempholder.h)

SET(PBRT_CORE_SOURCE
        src/core/geometry.cpp
        src/core/transform.cpp
        src/core/quaternion.cpp
        src/core/error.cpp
        src/core/api.cpp
        src/core/interaction.cpp
        src/core/shape.cpp
        src/core/efloat.cpp
        src/shapes/curve.cpp
        src/core/memory.cpp
        src/core/primitives.cpp
        src/core/material.cpp)

# Set files
# todo: complete file list
FILE(GLOB PBRT_SOURCE
        src/ext/*.c
        src/ext/*.cpp
        src/shapes/*.cpp
        )

# Add additional directories
# todo: complete directory list
INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(src/core)


ADD_LIBRARY(pbrt STATIC
        ${PBRT_YACC_LEX_SOURCE}
        ${PBRT_CORE_SOURCE}
        ${PBRT_CORE_HEADERS}
        ${PBRT_SOURCE}
        )
ADD_SANITIZERS(pbrt)

IF (WIN32)
    # Avoid a name clash when building on Visual Studio
    SET_TARGET_PROPERTIES(pbrt
            PROPERTIES
            OUTPUT_NAME libpbrt
            )
ENDIF ()


# Main renderer
ADD_EXECUTABLE(pbrt_exe
        src/main/pbrt.cpp
        )
ADD_SANITIZERS(pbrt_exe)
SET_TARGET_PROPERTIES(pbrt_exe
        PROPERTIES
        OUTPUT_NAME pbrt
        )
TARGET_LINK_LIBRARIES(pbrt_exe
        pbrt
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENEXR_LIBS}
        glog
        )

# todo: add executable for test and install commands
