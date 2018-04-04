cmake_minimum_required(VERSION 2.8.10)

project(nuphaseroot)
set(libname "nuphaseroot")

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})


if(DEFINED ENV{NUPHASE_INSTALL_DIR})
    message("NUPHASE_INSTALL_DIR is set to $ENV{NUPHASE_INSTALL_DIR}")
    set(UTIL_LIB_DIR $ENV{NUPHASE_INSTALL_DIR}/lib)
    set(UTIL_INC_DIR $ENV{NUPHASE_INSTALL_DIR}/include)
    set(UTIL_BIN_DIR $ENV{NUPHASE_INSTALL_DIR}/bin)
    set(UTIL_SHARE_DIR $ENV{NUPHASE_INSTALL_DIR}/share)
    set(LD_UTIL $ENV{NUPHASE_INSTALL_DIR}/lib)
    set(INC_UTIL $ENV{NUPHASE_INSTALL_DIR}/include)
else()
    message("Will install to /usr/local  NUPHASE_INSTALL_DIR not set")
    set(UTIL_LIB_DIR /usr/local/lib)
    set(UTIL_INC_DIR /usr/local/include)
    set(UTIL_BIN_DIR /usr/local/bin)
    set(UTIL_SHARE_DIR /usr/local/share)
    set(LD_UTIL /usr/local/lib)
    set(INC_UTIL /usr/local/include)
endif()


#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED)


#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
message("ROOT_VERSION is set to ${ROOT_VERSION}")
if( ${ROOT_VERSION} VERSION_GREATER "5.99/99")
    message("Using ROOT_VERSION 6")
    include(${ROOT_USE_FILE})
else()
#    include(RootNewMacros)
     add_definitions(${ROOT_DEFINITIONS})	
endif()



include_directories(${PROJECT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include ${UTIL_INC_DIR})

file(GLOB HEADER_FILES
    "include/*.h"    
)

file(GLOB SOURCE_FILES 
   "src/*.cc" 
)

set(DICTNAME G__${libname})


ROOT_GENERATE_DICTIONARY(${DICTNAME} ${HEADER_FILES} LINKDEF LinkDef.h)

#---Create a shared library with geneated dictionary
add_library(${libname} SHARED ${SOURCE_FILES} ${DICTNAME}.cxx)
target_link_libraries(${libname} ${ZLIB_LIBRARIES} ${ROOT_LIBRARIES} ${FFTW_LIBRARIES})

if( ${ROOT_VERSION} VERSION_GREATER "5.99.99")
    add_custom_target(${DICTNAME}.pcm DEPENDS ${DICTNAME})
endif()



install (FILES ${HEADER_FILES} DESTINATION ${UTIL_INC_DIR})
install (TARGETS ${libname}
	ARCHIVE DESTINATION ${UTIL_LIB_DIR}
	LIBRARY DESTINATION ${UTIL_LIB_DIR}
	RUNTIME DESTINATION ${UTIL_BIN_DIR})
      
if( ${ROOT_VERSION} VERSION_GREATER "5.99.99")
  install (FILES ${PROJECT_BINARY_DIR}/lib${libname}.rootmap DESTINATION ${UTIL_LIB_DIR})
endif() 



set(CMAKE_CXX_FLAGS_DEFAULT 
  "-Os -g -Wall" 
  CACHE STRING "c++ Flags used during default eventReaderRoot builds" 
  FORCE ) 

set(CMAKE_C_FLAGS_DEFAULT 
  "-Os -g -Wall"
  CACHE STRING "c Flags used during default eventReaderRoot builds" 
  FORCE ) 

set(CMAKE_EXE_LINKER_FLAGS_DEFAULT 
  "-g"
  CACHE STRING "ld Flags used during default eventReaderRoot builds" 
  FORCE ) 

set(CMAKE_SHARED_LINKER_FLAGS_DEFAULT 
  "-g"
  CACHE STRING "ld Flags used during default eventReaderRoot builds" 
  FORCE ) 


mark_as_advanced ( CMAKE_CXX_FLAGS_DEFAULT CMAKE_C_FLAGS_DEFAULT CMAKE_EXE_LINKER_FLAGS_DEFAULT CMAKE_SHARED_LINKER_FLAGS_DEFAULT) 

if (NOT CMAKE_BUILD_TYPE) 
  set (CMAKE_BUILD_TYPE Default
    CACHE STRING "Choose tye type of build: None Debug Release RelWithDebInfo MinSizeRel Default"
    FORCE ) 
endif()

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )


option (LIBNUPHASE_SUPPORT "Support for reading / converting from raw files." OFF) 
set (LIBNUPHASE_PATH  "" CACHE String "Additional path  to search for libnuphase (only necessary if not in NUPHASE_INSTALL_DIR)" ) 
set (EXTRA_LIBS "") 

if (LIBNUPHASE_SUPPORT) 
  add_definitions( -DHAVE_LIBNUPHASE )
  find_library(NUPHASE_LIB nuphase PATHS ${LIBNUPHASE_PATH})
  find_package(ZLIB REQUIRED)
  if (LIBNUPHASE_PATH STREQUAL "") 
    message ("Assuming libnuphase is in NUPHASE_INSTALL_DIR") 
  else()
    include_directories(${LIBNUPHASE_PATH}) 
    target_link_libraries(${libname} ${NUPHASE_LIB} ${ZLIB_LIBRARIES}) 
  endif()
endif(LIBNUPHASE_SUPPORT) 


set (BINSRCDIR programs) 
macro(add_program binary_name ) 
  add_executable(${binary_name} ${BINSRCDIR}/${binary_name}.cc) 
  target_link_libraries(${binary_name}  ${ROOT_LIBRARIES} ${libname})
  if (LIBNUPHASE_SUPPORT)
    target_link_libraries(${binary_name}  ${ZLIB_LIBRARIES} nuphase)
  endif()
  INSTALL (PROGRAMS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${binary_name} DESTINATION ${UTIL_BIN_DIR})
endmacro() 

add_program(nuphaseroot-convert) 
add_program(nuphaseroot-decimate) 



