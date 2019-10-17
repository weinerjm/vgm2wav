# Rules for building the demo.  Doesn't use an installed gme if you've already
# installed the project so if you're copying these rules you probably don't
# even need these next two lines if you're building against system-installed
# gme.
# Works with 3.11 and tested through 3.15
cmake_minimum_required(VERSION 3.11...3.15)

project(vgm2wavProject
    DESCRIPTION "Video game music to WAV"
    LANGUAGES C CXX)

# FetchContent added in CMake 3.11, downloads during the configure step
include(FetchContent)
# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()


FetchContent_Declare(
    libgme
    GIT_REPOSITORY  https://github.com/mcfiredrill/libgme.git
    GIT_TAG         ae76c88be9beb5132ee0e50c5006bbcb2ccb798e
)
FetchContent_MakeAvailable(libgme)

# step 0
find_path(GME_INCLUDE_DIR libgme/gme)

if((NOT GME_INCLUDE_DIR) OR (NOT EXISTS ${GME_INCLUDE_DIR}))
    # we couldn't find the header files for FOO or they don't exist
    message("Unable to find gme")

    # we have a submodule setup for foo, assume it is under external/foo
    # now we need to clone this submodule

    # set FOO_INCLUDE_DIR properly
    set(GME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libgme/gme
        CACHE PATH "gme include directory")

    # also install it
    install(DIRECTORY ${GME_INCLUDE_DIR}/ DESTINATION /usr/local/include)

    # for convenience setup a target
    add_library(gmelib INTERFACE)
    target_include_directories(gmelib INTERFACE
        $<BUILD_INTERFACE:${GME_INCLUDE_DIR}>
                               $<INSTALL_INTERFACE:/usr/local/include>)

    # need to export target as well
    install(TARGETS gmelib EXPORT my_export_set DESTINATION /usr/local/include)
else()
    add_library(libgme INTERFACE)
    target_include_directories(libgme INTERFACE ${GME_INCLUDE_DIR})
    target_link_libraries(libgme INTERFACE ${GME_LIBRARY})
endif()

add_subdirectory(app)
