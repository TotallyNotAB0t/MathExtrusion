# Install script for directory: C:/Users/pierr/Downloads/Cours/Maths/Extrusion/MathExtrusion/Extrusion/ThirdParty/stb-cmake

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/pierr/Downloads/Cours/Maths/Extrusion/MathExtrusion/Extrusion/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/debug" TYPE STATIC_LIBRARY MESSAGE_LAZY FILES "C:/Users/pierr/Downloads/Cours/Maths/Extrusion/MathExtrusion/Extrusion/out/build/x64-Debug/ThirdParty/stb-cmake/lib/stb_Debug.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE MESSAGE_LAZY FILES
    "C:/Users/pierr/Downloads/Cours/Maths/Extrusion/MathExtrusion/Extrusion/ThirdParty/stb-cmake/stb_image.h"
    "C:/Users/pierr/Downloads/Cours/Maths/Extrusion/MathExtrusion/Extrusion/ThirdParty/stb-cmake/stb_image_write.h"
    )
endif()
