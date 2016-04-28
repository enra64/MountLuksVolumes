cmake_minimum_required(VERSION 3.5)
project(OpenEncryptedDrives)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})


set(SOURCE_FILES main.cpp)
add_executable(OpenEncryptedDrives ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES})

add_custom_command(TARGET OpenEncryptedDrives POST_BUILD        # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/dialog.glade"      # <--this is in-file
        $<TARGET_FILE_DIR:OpenEncryptedDrives>)                 # <--this is out-file path
