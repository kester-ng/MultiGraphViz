cmake_minimum_required(VERSION 3.5)

project(MultiGraphViz VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Boost REQUIRED regex date_time system filesystem thread graph)
include_directories(${BOOST_INCLUDE_DIRS})
find_package(Graphviz REQUIRED)

find_package(OpenMP REQUIRED)
if(NOT TARGET OpenMP::OpenMP_CXX)
    add_library(OpenMP_TARGET INTERFACE)
    add_library(OpenMP::OpenMP_CXX ALIAS OpenMP_TARGET)
    target_compile_options(OpenMP_TARGET INTERFACE ${OpenMP_CXX_FLAGS})
    find_package(Threads REQUIRED)
    target_link_libraries(OpenMP_TARGET INTERFACE Threads::Threads)
    target_link_libraries(OpenMP_TARGET INTERFACE ${OpenMP_CXX_FLAGS})
endif()

qt_wrap_ui(mainwindow mainwindow.ui)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        graph/private/QGVNodePrivate.h
        graph/private/QGVNodePrivate.cpp
        graph/private/QGVCore.h
        graph/private/QGVCore.cpp
        graph/private/QGVEdgePrivate.h
        graph/private/QGVEdgePrivate.cpp
        graph/private/QGVGraphPrivate.h
        graph/private/QGVGraphPrivate.cpp
        graph/private/QGVGvcPrivate.h
        graph/private/QGVGvcPrivate.cpp
        graph/QGVEdge.h
        graph/QGVEdge.cpp
        graph/qgv.h
        graph/QGVScene.h
        graph/QGVScene.cpp
        graph/QGVNode.h
        graph/QGVNode.cpp
        graph/QGVSubGraph.h
        graph/QGVSubGraph.cpp
        louvain/graph.h
        louvain/graph.cpp
        louvain/graph_binary.h
        louvain/graph_binary.cpp
        louvain/community.h
        louvain/community.cpp
        louvain/main_community.cpp
        louvain/main_convert.cpp
        louvain/main_convert.h
        louvain/main_community.h
        QGraphicsViewEc.cpp
        QGraphicsViewEc.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MultiGraphViz
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MultiGraphViz APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MultiGraphViz SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MultiGraphViz
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MultiGraphViz PRIVATE Qt${QT_VERSION_MAJOR}::Widgets cgraph gvc OpenMP::OpenMP_CXX)

set_target_properties(MultiGraphViz PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MultiGraphViz)
endif()
