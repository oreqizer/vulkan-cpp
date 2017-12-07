# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindVulkan
# ----------
#
# Try to find Vulkan
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines :prop_tgt:`IMPORTED` target ``Vulkan::Vulkan``, if
# Vulkan has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables::
#
#   Vulkan_FOUND          - True if Vulkan was found
#   Vulkan_INCLUDE_DIRS   - include directories for Vulkan
#   Vulkan_LIBRARIES      - link against this library to use Vulkan
#
# The module will also define two cache variables::
#
#   Vulkan_INCLUDE_DIR    - the Vulkan include directory
#   Vulkan_LIBRARY        - the path to the Vulkan library
#

if (WIN32)
    find_path(Vulkan_INCLUDE_DIR
            NAMES vulkan/vulkan.h
            PATHS
            "$ENV{VULKAN_SDK}/Include")

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(Vulkan_LIBRARY
                NAMES vulkan-1
                PATHS
                "$ENV{VULKAN_SDK}/Lib"
                "$ENV{VULKAN_SDK}/Bin")
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
        find_library(Vulkan_LIBRARY
                NAMES vulkan-1
                PATHS
                "$ENV{VULKAN_SDK}/Lib32"
                "$ENV{VULKAN_SDK}/Bin32"
                NO_SYSTEM_ENVIRONMENT_PATH)
    endif ()
elseif (APPLE)
    find_library(Vulkan_LIBRARY MoltenVK)
    find_path(Vulkan_INCLUDE_DIR
            NAMES vulkan/vulkan.h
            HINTS "${Vulkan_LIBRARY}/Headers")

    # NOT PRETTY BUT A TEMP SOLUTION
    set(Vulkan_INCLUDE_DIR "/usr/local/include/MoltenVK")
else ()
    find_path (Vulkan_INCLUDE_DIR
            NAMES vulkan/vulkan.h
            PATHS
            "$ENV{VULKAN_SDK}/include")
    find_library (Vulkan_LIBRARY
            NAMES vulkan
            PATHS
            "$ENV{VULKAN_SDK}/lib")
endif ()

set(Vulkan_LIBRARIES ${Vulkan_LIBRARY})
set(Vulkan_INCLUDE_DIRS ${Vulkan_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vulkan DEFAULT_MSG Vulkan_LIBRARY Vulkan_INCLUDE_DIR)

mark_as_advanced(Vulkan_INCLUDE_DIR Vulkan_LIBRARY Vulkan_STATIC_LIBRARY)
