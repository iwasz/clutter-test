# Wymagaj tej wersji - dopier w niej jest nowe makro FindPkgConfig
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.0 FATAL_ERROR)

PROJECT (clutter-test)
#SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)
INCLUDE (../tiliae/tiliae.cmake)
SET (CMAKE_VERBOSE_MAKEFILE ON)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11" CACHE INTERNAL "cxx compiler flags")

SET(CMAKE_C_FLAGS_RELEASE "-DNDEBUG" CACHE INTERNAL "c release compiler flags")
SET(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG" CACHE INTERNAL "cxx release compiler flags")

ADD_DEFINITIONS (-DBOOST_LOG_DYN_LINK)

find_package (Boost REQUIRED COMPONENTS log log_setup regex)
include_directories(${Boost_INCLUDE_DIRS})
link_directories (${Boost_LIBRARY_DIRS})

include (FindPkgConfig)

IF (0)
pkg_check_modules (TILIAE "tiliae")
if (NOT TILIAE_FOUND)
        MESSAGE (STATUS "Tiliae not installed, using local copy.")
        SET (TILIAE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../tiliae/src/")
        SET (TILIAE_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/../tiliae/build/src/")
        SET (TILIAE_LIBRARIES "tiliae")
        MESSAGE (STATUS ${TILIAE_INCLUDE_DIRS})
        MESSAGE (STATUS ${TILIAE_LIBRARY_DIRS})
endif ()
INCLUDE_DIRECTORIES (${TILIAE_INCLUDE_DIRS})
link_directories(${TILIAE_LIBRARY_DIRS})

pkg_check_modules (GTKFORMS "gtkforms")
if (NOT GTKFORMS_FOUND)
        MESSAGE (STATUS "GtkForms not installed, using local copy.")
        SET (GTKFORMS_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../gtkforms/src/")
        SET (GTKFORMS_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/../gtkforms/build/")
        SET (GTKFORMS_LIBRARIES "gtkforms")
        MESSAGE (STATUS ${GTKFORMS_INCLUDE_DIRS})
        MESSAGE (STATUS ${GTKFORMS_LIBRARY_DIRS})
endif ()
INCLUDE_DIRECTORIES (${GTKFORMS_INCLUDE_DIRS})
link_directories(${GTKFORMS_LIBRARY_DIRS})
endif()

pkg_check_modules (CLUTTER_GTK REQUIRED "clutter-gtk-1.0")
INCLUDE_DIRECTORIES (${CLUTTER_GTK_INCLUDE_DIRS})
link_directories(${CLUTTER_GTK_LIBRARY_DIRS})

# +-------------------------------+
# | Generate reflection database. |
# +-------------------------------+
#TILIAE_PARSE ("${PROJECT_SOURCE_DIR}/../tiliae/build/parser/tiliaeparser" "gtkFormsDemo" "src/Reflection_input.cc" "src/Reflection_output.cc.h")
#LIST (APPEND SOURCES "src/Reflection_input.cc")
#LIST (APPEND SOURCES "src/Reflection_output.cc.h")

# +-------------------------------+
# | Main target                   |
# +-------------------------------+

SET (SOURCES "src/main.cc")

# Local includes (inside project).
INCLUDE_DIRECTORIES (src)
INCLUDE_DIRECTORIES ("/usr/include/libcxxabi")

add_executable (${CMAKE_PROJECT_NAME} ${SOURCES})

TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME} c++abi)
#TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME} pthread)
TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME} ${TILIAE_LIBRARIES})
TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME} ${CLUTTER_GTK_LIBRARIES})
#TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME} ${GTKFORMS_LIBRARIES})


