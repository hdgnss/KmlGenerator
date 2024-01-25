cmake_minimum_required(VERSION 3.5)

project(KmlGenerator VERSION 1.0.0 LANGUAGES CXX)

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h")

include_directories(${PROJECT_BINARY_BIN})
include_directories(lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql)

if (WIN32)
    set(APP_ICONS "${CMAKE_CURRENT_SOURCE_DIR}/resources/launcher.rc")
elseif (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0)
endif()

if (BUILD_WITH_STATIC_LIB STREQUAL "ON")
  add_library(KmlGenerator STATIC
    kmlgenerator.cpp
    kmlgenerator.h
  )
else()
  if (WIN32)
    add_executable(KmlGenerator
      main.cpp
      kmlgenerator.cpp
      ${APP_ICONS}
    )
  elseif (APPLE)
    add_executable(KmlGenerator
      main.cpp
      kmlgenerator.cpp
    )
  endif()
endif()

target_link_libraries(KmlGenerator Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql)
if (WIN32)
  target_link_libraries(KmlGenerator ${CMAKE_SOURCE_DIR}/lib/NmeaBase.lib)
elseif (APPLE)
  target_link_libraries(KmlGenerator ${CMAKE_SOURCE_DIR}/lib/libNmeaBase.a)
endif()


set_target_properties(KmlGenerator PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_GUI_IDENTIFIER com.hdgnss.logconverter
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_COPYRIGHT "Copyright 2024, HDGNSS. All Rights Reserved." 
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS KmlGenerator
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
