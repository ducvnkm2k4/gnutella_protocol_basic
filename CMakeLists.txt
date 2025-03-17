cmake_minimum_required(VERSION 3.14)

if (WIN32)
    project(GNUTELLA_PJ LANGUAGES CXX)
elseif(UNIX)
    project(GNUTELLA_PJ)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)

#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
set(CMAKE_BUILD_TYPE Release)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)

#=================== INCLUSION OF Project Files ====================#
set(UI_DIR "${CMAKE_SOURCE_DIR}/ui")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

include_directories(${UI_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${UI_DIR}/*.ui"
    "${UI_DIR}/*.qrc"
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

#=================== SETUP EXECUTABLE ====================#
# Enable debug logging on RELWITHDEBINFO configuration
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${UI_DIR})

# Thiết lập thư mục đầu ra cho file thực thi
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add executable
add_executable(GNUTELLA_PJ ${SOURCES})

# Add the target includes for GNUTELLA_PJ 
target_include_directories(GNUTELLA_PJ PRIVATE ${UI_DIR})
target_include_directories(GNUTELLA_PJ PRIVATE ${INCLUDE_DIR})
target_include_directories(GNUTELLA_PJ PRIVATE ${SOURCE_DIR})

#===================== LINKING LIBRARIES =======================#
if (WIN32)
    target_link_libraries(GNUTELLA_PJ Qt6::Widgets ws2_32)
else()
    target_link_libraries(GNUTELLA_PJ Qt6::Widgets)
endif()
