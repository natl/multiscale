# $Id: CMakeLists.txt 68058 2013-03-13 14:47:43Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(macro)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only exmacroable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
MESSAGE( STATUS "Geant4_INCLUDE_DIR: " ${Geant4_INCLUDE_DIR} )

#----------------------------------------------------------------------------
# Add the exmacroable, and link it to the Geant4 libraries
#

add_executable(macro macro.cc ${sources} ${headers})
target_link_libraries(macro ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the exmacroable directly because it
# relies on these scripts being in the current working directory.
#
#This line creates a set of values which are the filenames to copy
#set( SETNAME item1 item2 ... )
set(ANALYSIS_SCRIPTS
  macroanalysis.C
  )

file(GLOB MACRO_FILES
    "*.mac"
    )

foreach(_script ${ANALYSIS_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

### Copying
FILE(COPY ${PROJECT_SOURCE_DIR}/testing DESTINATION ${PROJECT_BINARY_DIR})
FILE(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})
### Make the data drectory for HepRep files
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
