CMAKE_MINIMUM_REQUIRED(VERSION 3.4 FATAL_ERROR)

PROJECT(Gearoenix)

SET(PROJECT_DESCRIPTION "A cross-platform C++ game engine.")

SET(GX_VERSION_MAJOR 0)
SET(GX_VERSION_MINOR 2)
SET(GX_VERSION_PATCH 0)
SET(GX_VERSION "${GX_VERSION_MAJOR}.${GX_VERSION_MINOR}.${GX_VERSION_PATCH}")

MESSAGE(STATUS "Gearoenix is going to be compiled for ${CMAKE_SYSTEM_NAME}")

# Project configurations
# System API selection
# For android default is native activity and for others SDL2
OPTION(GX_SYSTEM_API "Use SDL2 library" ON)
IF (CMAKE_SYSTEM_NAME MATCHES "Android")
    SET(GX_DEFAULT_SYSTEM_API "Native")
ELSE ()
    SET(GX_DEFAULT_SYSTEM_API "SDL2")
ENDIF ()
SET(GX_SYSTEM_API "${GX_DEFAULT_SYSTEM_API}" CACHE STRING "Gearoenix System API that interact with operating system." FORCE)
SET_PROPERTY(CACHE GX_SYSTEM_API PROPERTY STRINGS "SDL2" "Native")
MESSAGE(STATUS "Gearoenix system API is ${GX_SYSTEM_API}")
SET(GX_USE_SDL2 OFF)
SET(GX_USE_NATIVE OFF)
IF (GX_SYSTEM_API MATCHES "Native")
    SET(GX_USE_NATIVE ON)
ELSEIF (GX_SYSTEM_API MATCHES "SDL2")
    SET(GX_USE_SDL2 ON)
ELSE ()
    MESSAGE(FATAL_ERROR "Unexpected system API selected.")
ENDIF ()
# OpenGL ES 2
OPTION(GX_USE_OPENGL_ES2 "Activate OpenGL ES 2" ON)
# OpenGL ES 3
OPTION(GX_USE_OPENGL_ES3 "Activate OpenGL ES 3" ON)
# OpenGL 3.3
OPTION(GX_USE_OPENGL_33 "Activate OpenGL 3.3" ON)
# OpenGL 4.3 (it has compute shader)
OPTION(GX_USE_OPENGL_43 "Activate OpenGL 4.3" ON)
# Vulkan API (work in progress)
OPTION(GX_USE_VULKAN "Activate Vulkan (WIP)" OFF)
IF (MSVC)
    OPTION(GX_USE_DIRECTX11 "Activate Direct3DX 11 (WIP)" OFF)
    OPTION(GX_USE_DIRECTX12 "Activate Direct3DX 12 (WIP)" OFF)
ENDIF ()
# Document generation (planned)
OPTION(GX_GENERATE_DOCUMENTS "Generate documentation (Planned)" OFF)
# Tools targets
OPTION(GX_ENABLE_TOOLS "Enables the tools" ON)
# Example targets
OPTION(GX_ENABLE_EXAMPLES "Enables the examples" ON)
# Test
OPTION(GX_ENABLE_TESTS "Enables the tests" OFF)
# Test coverage
OPTION(GX_ENABLE_COVERAGE "Enables the measurement of test coverages" OFF)
# Demo World Wide Racing
OPTION(GX_ENABLE_DEMO_WORLD_WIDE_RACING "Enables the compilation of World Wide Racing game demo" OFF)
# Libraries that are dependencies of the project
SET(GX_LIBS)
# Configuring the language level
IF (MSVC)
    SET(CMAKE_CXX_FLAGS "/std:c++latest ${CMAKE_CXX_FLAGS}")
ELSE ()
    SET(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS}")
ENDIF ()
IF (APPLE)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
ENDIF ()
# Cmake modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
# Test coverage
IF (GX_ENABLE_TESTS AND GX_ENABLE_COVERAGE)
    INCLUDE(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    SET(COVERAGE_EXCLUDES
            "/usr/*"
            "${CMAKE_SOURCE_DIR}/examples/*"
            "${CMAKE_SOURCE_DIR}/sdk/*"
            "${CMAKE_SOURCE_DIR}/tests/*")
    SETUP_TARGET_FOR_COVERAGE(NAME GX_COVERAGE EXECUTABLE ctest DEPENDENCIES GearoenixTests)
ENDIF (GX_ENABLE_TESTS AND GX_ENABLE_COVERAGE)
# Warning generation
IF (MSVC)
    STRING(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS_NEW ${CMAKE_CXX_FLAGS})
    IF(${CMAKE_CXX_FLAGS_NEW} STREQUAL ${CMAKE_CXX_FLAGS})
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    ELSE()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_NEW}")
    ENDIF()
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
ELSE ()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
ENDIF (MSVC)
# Webassembly customized flags
IF (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    MESSAGE(STATUS "SDL2 dependency added for Emscripten.")
    SET(GX_EMS_FLAGS "-s WASM=1 -s NO_EXIT_RUNTIME=1 -s BINARYEN=1 -s USE_SDL=2 --no-heap-copy -Os")
    SET(GX_EMS_FLAGS "${GX_EMS_FLAGS} -s INITIAL_MEMORY=268435456 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2")
    SET(GX_EMS_FLAGS "${GX_EMS_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/assets@/")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GX_EMS_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GX_EMS_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GX_EMS_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GX_EMS_FLAGS}")
    SET(CMAKE_EXECUTABLE_SUFFIX .html)
    SET(GX_LIBS ${GX_LIBS} ${GX_EMS_FLAGS})
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Emscripten")

SET(GX_SDK_PATH "${CMAKE_SOURCE_DIR}/sdk")
MESSAGE(STATUS "Gearoenix SDK path is '${GX_SDK_PATH}'.")
SET(GX_SDK_LIB_PATH "${GX_SDK_PATH}/libs/")
MESSAGE(STATUS "Gearoenix SDK libs path is '${GX_SDK_LIB_PATH}'.")
SET(GX_SDK_IOS_LIB_PATH "${GX_SDK_LIB_PATH}ios/")
SET(GX_BOOST_INCLUDE_DIRS "${GX_SDK_PATH}/boost")
SET(GX_GLM_INCLUDE_DIRS "${GX_SDK_PATH}/glm")
SET(GX_SDL2_PATH "${GX_SDK_PATH}/SDL2")
SET(GX_SDL2_INCLUDE_DIRS "${GX_SDL2_PATH}/include")
MESSAGE(STATUS "Gearoenix SDK SDL2 include path is '${GX_SDL2_INCLUDE_DIRS}'.")
SET(GX_STB_INCLUDE_DIRS "${GX_SDK_PATH}/stb")
# Build configuration
SET(GX_DEFAULT_BUILD_TYPE "MinSizeRel")
if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
    SET(GX_DEFAULT_BUILD_TYPE "Debug")
endif ()
IF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    MESSAGE(STATUS "Setting build type to '${GX_DEFAULT_BUILD_TYPE}' as none was specified.")
    SET(CMAKE_BUILD_TYPE "${GX_DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
# Include directories
INCLUDE_DIRECTORIES(${GX_STB_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GX_VMA_INCLUDE_DIRS})

IF(DEFINED ENV{VULKAN_SDK})
    INCLUDE_DIRECTORIES($ENV{VULKAN_SDK}/Include)
ENDIF()

IF (GX_USE_SDL2)
    ADD_DEFINITIONS(-DGX_USE_SDL)
    INCLUDE_DIRECTORIES(${GX_SDL2_INCLUDE_DIRS})
    IF (MSVC)
        SET(GX_LIBS ${GX_LIBS} "${GX_SDK_LIB_PATH}SDL2-static.lib" "${GX_SDK_LIB_PATH}SDL2main.lib")
        SET(GX_LIBS ${GX_LIBS} winmm version oleaut32 imm32 setupapi)
    ELSEIF (APPLE)
        FIND_LIBRARY(CORE_AUDIO_LIBRARY CoreAudio)
        MARK_AS_ADVANCED(CORE_AUDIO_LIBRARY)
        SET(GX_LIBS ${GX_LIBS} ${CORE_AUDIO_LIBRARY})
        FIND_LIBRARY(CORE_VIDEO_LIBRARY CoreVideo)
        MARK_AS_ADVANCED(CORE_VIDEO_LIBRARY)
        SET(GX_LIBS ${GX_LIBS} ${CORE_VIDEO_LIBRARY})
        FIND_LIBRARY(CORE_SERVICES_LIBRARY CoreServices)
        MARK_AS_ADVANCED(CORE_SERVICES_LIBRARY)
        SET(GX_LIBS ${GX_LIBS} ${CORE_SERVICES_LIBRARY})
        FIND_LIBRARY(AUDIO_TOOLBOX_LIBRARY AudioToolbox)
        MARK_AS_ADVANCED(AUDIO_TOOLBOX_LIBRARY)
        SET(GX_LIBS ${GX_LIBS} ${AUDIO_TOOLBOX_LIBRARY})
        FIND_LIBRARY(IO_KIT_LIBRARY IOKit)
        MARK_AS_ADVANCED(IO_KIT_LIBRARY)
        SET(GX_LIBS ${GX_LIBS} ${IO_KIT_LIBRARY})
        SET(GX_LIBS ${GX_LIBS} iconv)
        IF (CMAKE_SYSTEM_NAME MATCHES "iOS")
            FIND_LIBRARY(UIKIT_LIBRARY UIKit)
            MARK_AS_ADVANCED(UIKIT_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${UIKIT_LIBRARY})
            FIND_LIBRARY(FOUNDATION_LIBRARY Foundation)
            MARK_AS_ADVANCED(FOUNDATION_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${FOUNDATION_LIBRARY})
            FIND_LIBRARY(GAME_CONTROLLER_LIBRARY GameController)
            MARK_AS_ADVANCED(GAME_CONTROLLER_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${GAME_CONTROLLER_LIBRARY})
            FIND_LIBRARY(OPENGLES_LIBRARY OpenGLES)
            MARK_AS_ADVANCED(OPENGLES_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${OPENGLES_LIBRARY})
            FIND_LIBRARY(AVFOUNDATION_LIBRARY AVFoundation)
            MARK_AS_ADVANCED(AVFOUNDATION_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${AVFOUNDATION_LIBRARY})
            FIND_LIBRARY(CORE_MOTION_LIBRARY CoreMotion)
            MARK_AS_ADVANCED(CORE_MOTION_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${CORE_MOTION_LIBRARY})
            FIND_LIBRARY(QUARTZ_CORE_LIBRARY QuartzCore)
            MARK_AS_ADVANCED(QUARTZ_CORE_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${QUARTZ_CORE_LIBRARY})
            FIND_LIBRARY(CORE_GRAPHICS_LIBRARY CoreGraphics)
            MARK_AS_ADVANCED(CORE_GRAPHICS_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${CORE_GRAPHICS_LIBRARY})
            FIND_LIBRARY(METAL_LIBRARY Metal)
            MARK_AS_ADVANCED(METAL_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${METAL_LIBRARY})
            SET(GX_LIBS ${GX_LIBS} "${GX_SDK_IOS_LIB_PATH}libSDL2.a")
        ELSE ()
            FIND_LIBRARY(COCOA_LIBRARY Cocoa)
            MARK_AS_ADVANCED(COCOA_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${COCOA_LIBRARY})
            FIND_LIBRARY(FORCE_FEEDBACK_LIBRARY ForceFeedback)
            MARK_AS_ADVANCED(FORCE_FEEDBACK_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${FORCE_FEEDBACK_LIBRARY})
            FIND_LIBRARY(CARBON_LIBRARY Carbon)
            MARK_AS_ADVANCED(CARBON_LIBRARY)
            SET(GX_LIBS ${GX_LIBS} ${CARBON_LIBRARY})
        ENDIF (CMAKE_SYSTEM_NAME MATCHES "iOS")
    ELSEIF (NOT CMAKE_SYSTEM_NAME MATCHES "Android" AND NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
        SET(GX_LIBS ${GX_LIBS} "${GX_SDK_LIB_PATH}libSDL2.a" "${GX_SDK_LIB_PATH}libSDL2main.a")
        SET(GX_LIBS ${GX_LIBS} pthread dl sndio m)
    ENDIF (MSVC)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Android")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
    SET(GX_NDK_HELPER_DIR "${ANDROID_NDK}/sources/android/ndk_helper")
    SET(GX_APP_GLUE "${ANDROID_NDK}/sources/android/native_app_glue")
    ADD_LIBRARY(ndk-helper STATIC ${GX_NDK_HELPER_DIR}/JNIHelper.cpp)
    TARGET_INCLUDE_DIRECTORIES(ndk-helper PRIVATE ${GX_APP_GLUE})
    ADD_LIBRARY(native-app-glue STATIC ${GX_APP_GLUE}/android_native_app_glue.c)
    SET(GX_LIBS ${GX_LIBS} ndk-helper native-app-glue android EGL log m)
    INCLUDE_DIRECTORIES(${GX_APP_GLUE} ${GX_NDK_HELPER_DIR})
ENDIF (GX_USE_SDL2)

ADD_SUBDIRECTORY(gearoenix)

IF (GX_ENABLE_EXAMPLES)
    ADD_SUBDIRECTORY(examples)
ENDIF (GX_ENABLE_EXAMPLES)

IF (GX_ENABLE_TOOLS)
    ADD_SUBDIRECTORY(tools)
ENDIF (GX_ENABLE_TOOLS)

IF (GX_ENABLE_TESTS)
    INCLUDE(CTest)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(tests)
ENDIF (GX_ENABLE_TESTS)

IF (GX_ENABLE_DEMO_WORLD_WIDE_RACING)
    ADD_SUBDIRECTORY(demos/world-wide-racing)
ENDIF(GX_ENABLE_DEMO_WORLD_WIDE_RACING)