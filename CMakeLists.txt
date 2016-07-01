# Rules for building the demo.  Doesn't use an installed gme if you've already
# installed the project so if you're copying these rules you probably don't
# even need these next two lines if you're building against system-installed
# gme.
cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_SOURCE_DIR}/gme ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_BINARY_DIR}/gme ${CMAKE_BINARY_DIR}/src)

add_executable(vgm2wav src/wave_writer.c src/main.c)

target_link_libraries(vgm2wav gme)
install(TARGETS vgm2wav DESTINATION /usr/local/bin)
