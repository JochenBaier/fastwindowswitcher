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


set( TARGET_NAME "FastWindowSwitcherLib" )
project( ${TARGET_NAME} )

cmake_minimum_required(VERSION 2.8.11)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set( SOURCE_FILES

${SOURCE_DIR}/FastWindowSwitcher.cpp
${SOURCE_DIR}/FastWindowSwitcher.hpp
${SOURCE_DIR}/IDesktopAbstraction.hpp
${SOURCE_DIR}/DesktopAbstraction.cpp 
${SOURCE_DIR}/DesktopAbstraction.hpp
${SOURCE_DIR}/MarkerFunctions.cpp
${SOURCE_DIR}/MarkerFunctions.hpp
${SOURCE_DIR}/MonitorInfo.hpp
${SOURCE_DIR}/MonitorFunctions.cpp
${SOURCE_DIR}/MonitorFunctions.hpp
${SOURCE_DIR}/PanelFunctions.cpp
${SOURCE_DIR}/PanelFunctions.hpp
${SOURCE_DIR}/SelectableElement.hpp
${SOURCE_DIR}/SelectableElementPtr.hpp
${SOURCE_DIR}/SelectableElementRepository.cpp
${SOURCE_DIR}/SelectableElementRepository.hpp
${SOURCE_DIR}/SelectableElementType.hpp
${SOURCE_DIR}/SelectablePanelElement.cpp
${SOURCE_DIR}/SelectablePanelElement.hpp
${SOURCE_DIR}/SelectableWindow.cpp
${SOURCE_DIR}/SelectableWindow.hpp
${SOURCE_DIR}/TaskbarPanel.hpp
${SOURCE_DIR}/UIElement.cpp
${SOURCE_DIR}/UIElement.hpp
${SOURCE_DIR}/UIAutomationFunctions.cpp
${SOURCE_DIR}/UIAutomationFunctions.hpp
${SOURCE_DIR}/UIControlViewWalker.cpp
${SOURCE_DIR}/UIControlViewWalker.hpp
${SOURCE_DIR}/WinApiFunctions.cpp
${SOURCE_DIR}/WinApiFunctions.hpp
${SOURCE_DIR}/WindowFunctions.cpp
${SOURCE_DIR}/WindowFunctions.hpp
${SOURCE_DIR}/SelectableWindowFunctions.hpp
${SOURCE_DIR}/SelectableWindowFunctions.cpp
${SOURCE_DIR}/SelectableElementFunctions.cpp
${SOURCE_DIR}/SelectableElementFunctions.cpp
${SOURCE_DIR}/SelectablePanelElementFunctions.cpp
${SOURCE_DIR}/SelectablePanelElementFunctions.hpp
)

include_directories(${SOURCE_DIR})

include_directories("${CMAKE_PREFIX_PATH}/include")
include_directories("${CMAKE_PREFIX_PATH}/include/QtWidgets")
include_directories("${CMAKE_PREFIX_PATH}/include/QtGui")
include_directories("${CMAKE_PREFIX_PATH}/include/QtCore")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets  REQUIRED)
add_library( ${TARGET_NAME} STATIC ${SOURCE_FILES} )


#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /std:c++latest")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /std:c++latest ")

target_compile_options( ${TARGET_NAME} PRIVATE "$<$<CONFIG:DEBUG>:/W4>"  )

target_compile_options( ${TARGET_NAME} PRIVATE "$<$<CONFIG:RELEASE>:/MT>" )
target_compile_options( ${TARGET_NAME} PRIVATE "$<$<CONFIG:DEBUG>:/MTd>" )


set(cfgs ${CMAKE_CONFIGURATION_TYPES}) 
list(REMOVE_ITEM cfgs MinSizeRel)
list(REMOVE_ITEM cfgs RelWithDebInfo)