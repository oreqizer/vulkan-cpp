cmake_minimum_required(VERSION 3.8)
project(vulkan-cpp)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Setting up PkgConfig
find_package(PkgConfig REQUIRED)

# Finding and linking GLFW3
pkg_search_module(GLFW3 3.2 REQUIRED glfw3)
if (GLFW3_FOUND)
    message(STATUS "Found GLFW, Including and Linking now")
    include_directories(${GLFW3_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLFW3_STATIC_LIBRARIES})
endif (GLFW3_FOUND)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Finding and linking GKM
pkg_search_module(GKM REQUIRED glm)
if (GKM_FOUND)
    message(STATUS "Found GLM, Including and Linking now")
    include_directories(${GKM_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GKM_STATIC_LIBRARIES})
endif (GKM_FOUND)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Finding and linking Vulkan
find_package(Vulkan)
if (VULKAN_FOUND)
    message(STATUS "Found Vulkan, Including and Linking now")
    include_directories(${Vulkan_INCLUDE_DIR})
    target_link_libraries (${PROJECT_NAME} ${Vulkan_LIBRARY})
endif (VULKAN_FOUND)
