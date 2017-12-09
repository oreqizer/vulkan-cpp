cmake_minimum_required(VERSION 3.8)
project(vulkan-cpp)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp src/instance.cpp src/instance.h src/debug.h src/debug.cpp src/surface.cpp src/surface.h src/devices.cpp src/devices.h src/swapchain.cpp src/swapchain.h src/queue.cpp src/queue.h src/views.cpp src/views.h)
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

# Finding and linking GLM
pkg_search_module(GLM REQUIRED glm)
if (GLM_FOUND)
    message(STATUS "Found GLM, Including and Linking now")
    include_directories(${GLM_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLM_STATIC_LIBRARIES})
endif (GLM_FOUND)

# Finding and linking Vulkan
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Vulkan)
if (Vulkan_FOUND)
    message(STATUS "Found Vulkan, Including and Linking now")
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${Vulkan_LIBRARIES})
endif (Vulkan_FOUND)
