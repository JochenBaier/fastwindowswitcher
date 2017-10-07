#***************************************************************************
#**
#** Copyright (C) 2017 Jochen Baier
#** Contact: email@jochen-baier.
#**
#** This file is part of the FastWindowSwitcher
#
#This program is free software : you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.If not, see < http://www.gnu.org/licenses/>.
#
#Dieses Programm ist Freie Software : Sie können es unter den Bedingungen
#der GNU General Public License, wie von der Free Software Foundation,
#Version 3 der Lizenz oder(nach Ihrer Wahl) jeder neueren
#veröffentlichten Version, weiterverbreiten und / oder modifizieren.
#
#Dieses Programm wird in der Hoffnung, dass es nützlich sein wird, aber
#OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
#Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
#Siehe die GNU General Public License für weitere Details.
#
#Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
#Programm erhalten haben.Wenn nicht, siehe < http://www.gnu.org/licenses/>.
#******************************************************************************


set( TARGET_NAME "FastWindowSwitcher" )

cmake_minimum_required(VERSION 2.8.11)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set( SOURCE_FILES
${SOURCE_DIR}/HiddenEventWindow.cpp
${SOURCE_DIR}/HiddenEventWindow.hpp
${SOURCE_DIR}/main.cpp
${SOURCE_DIR}/RunGuard.cpp
${SOURCE_DIR}/Resources.qrc
${SOURCE_DIR}/RunGuard.hpp
${SOURCE_DIR}/MarkerWindow.cpp
${SOURCE_DIR}/MarkerWindow.hpp
${SOURCE_DIR}/TrayIcon.cpp
${SOURCE_DIR}/TrayIcon.hpp
${SOURCE_DIR}/Session.hpp
${SOURCE_DIR}/Session.cpp
${SOURCE_DIR}/SessionManager.hpp
${SOURCE_DIR}/SessionManager.cpp
${SOURCE_DIR}/PersistentSettings.hpp
${SOURCE_DIR}/VersionInfo.rc #FIXME use https://stackoverflow.com/questions/6693100/how-to-generate-windows-dll-versioning-information-with-cmake
${SOURCE_DIR}/XmlFunctions.cpp
${SOURCE_DIR}/XmlFunctions.hpp
${SOURCE_DIR}/HotKeyFunctions.cpp 
${SOURCE_DIR}/HotKeyFunctions.hpp 
${SOURCE_DIR}/MiniDumpFunctions.cpp
${SOURCE_DIR}/MiniDumpFunctions.hpp
${SOURCE_DIR}/WelcomeDialogFunctions.cpp
${SOURCE_DIR}/WelcomeDialogFunctions.hpp
${SOURCE_DIR}/SettingsWindow.cpp
${SOURCE_DIR}/SettingsWindow.hpp
${SOURCE_DIR}/PersistentSettingsFunctions.cpp
${SOURCE_DIR}/PersistentSettingsFunctions.hpp
${SOURCE_DIR}/SettingsDialog.ui
${SOURCE_DIR}/WelcomeDialog.ui
)

include_directories(${SOURCE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTORCC ON) 
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)

add_executable(${TARGET_NAME} WIN32 ${SOURCE_FILES} )

target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets)
target_link_libraries(${TARGET_NAME} PRIVATE Qt5::XmlPatterns)

add_library( "qwindows" STATIC IMPORTED )
set_target_properties( "qwindows" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/plugins/platforms/qwindows.lib")
set_target_properties( "qwindows" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/plugins/platforms/qwindowsd.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE "qwindows")

add_library( "qdirect2d" STATIC IMPORTED )
set_target_properties( "qdirect2d" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/plugins/platforms/qdirect2d.lib")
set_target_properties( "qdirect2d" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/plugins/platforms/qdirect2dd.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE "qdirect2d")

add_library( "qtharfbuzz" STATIC IMPORTED )
set_target_properties( "qtharfbuzz" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/lib/qtharfbuzz.lib")
set_target_properties( "qtharfbuzz" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/lib/qtharfbuzzd.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE "qtharfbuzz")

add_library( "qtfreetype" STATIC IMPORTED )
set_target_properties( "qtfreetype" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/lib/qtfreetype.lib")
set_target_properties( "qtfreetype" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/lib/qtfreetyped.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE "qtfreetype")

add_library( "Qt5AccessibilitySupport" STATIC IMPORTED )
set_target_properties( "Qt5AccessibilitySupport" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/lib/Qt5AccessibilitySupport.lib")
set_target_properties( "Qt5AccessibilitySupport" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/lib/Qt5AccessibilitySupportd.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE "Qt5AccessibilitySupport")

add_library( "Qt5FontDatabaseSupport" STATIC IMPORTED )
set_target_properties( "Qt5FontDatabaseSupport" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/lib/Qt5FontDatabaseSupport.lib")
set_target_properties( "Qt5FontDatabaseSupport" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/lib/Qt5FontDatabaseSupportd.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE "Qt5FontDatabaseSupport")

add_library( "qtpcre2" STATIC IMPORTED )
set_target_properties( "qtpcre2" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/lib/qtpcre2.lib")
set_target_properties( "qtpcre2" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/lib/qtpcre2d.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE "qtpcre2")

add_library( "Qt5EventDispatcherSupport" STATIC IMPORTED )
set_target_properties( "Qt5EventDispatcherSupport" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/lib/Qt5EventDispatcherSupport.lib")
set_target_properties( "Qt5EventDispatcherSupport" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/lib/Qt5EventDispatcherSupportd.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE  "Qt5EventDispatcherSupport")

add_library( "Qt5ThemeSupport" STATIC IMPORTED )
set_target_properties( "Qt5ThemeSupport" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/lib/Qt5ThemeSupport.lib")
set_target_properties( "Qt5ThemeSupport" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/lib/Qt5ThemeSupportd.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE "Qt5ThemeSupport")

add_library( "qtlibpng" STATIC IMPORTED )
set_target_properties( "qtlibpng" PROPERTIES IMPORTED_LOCATION_RELEASE "${CMAKE_PREFIX_PATH}/lib/qtlibpng.lib")
set_target_properties( "qtlibpng" PROPERTIES IMPORTED_LOCATION_DEBUG "${CMAKE_PREFIX_PATH}/lib/qtlibpngd.lib")
target_link_libraries( ${TARGET_NAME} PRIVATE "qtlibpng")


target_link_libraries(${TARGET_NAME}  PRIVATE imm32.lib winmm.lib Ws2_32.lib UxTheme.lib Dwmapi.lib version.lib Iphlpapi.lib)

#FIXME switch to VS2017 compiler if bug https://developercommunity.visualstudio.com/content/problem/76198/vs-2017-compiler-creates-broken-debug-build-using.html 
#is fixed
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /std:c++latest")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /std:c++latest ")

target_compile_options( ${TARGET_NAME} PRIVATE "$<$<CONFIG:DEBUG>:/W4>"  )

target_compile_options( ${TARGET_NAME} PRIVATE "$<$<CONFIG:RELEASE>:/MT>" )
target_compile_options( ${TARGET_NAME} PRIVATE "$<$<CONFIG:DEBUG>:/MTd>" )
target_compile_options( ${TARGET_NAME} PRIVATE "/MP")

target_compile_options( ${TARGET_NAME} PRIVATE "/Zi")
target_compile_options( ${TARGET_NAME} PRIVATE "/Od")
target_compile_options( ${TARGET_NAME} PRIVATE "/Ob0")

#from  https://msdn.microsoft.com/en-us/library/windows/desktop/ee416349(v=vs.85).aspx
set_target_properties( ${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE "/DEBUG /OPT:REF /OPT:ICF" )


set(cfgs ${CMAKE_CONFIGURATION_TYPES}) 
list(REMOVE_ITEM cfgs MinSizeRel)
list(REMOVE_ITEM cfgs RelWithDebInfo)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../fast_window_switcher_lib/src)
include_directories(${CMAKE_BINARY_DIR} ) #for ui_ files

#include_directories("C:/Program Files (x86)/Visual Leak Detecto/include") #FIXME
#target_include_directories( ${TARGET_NAME} PRIVATE "C:/Program Files (x86)/Visual Leak Detector/lib/Win32")

target_link_libraries(${TARGET_NAME} PRIVATE "FastWindowSwitcherLib")
