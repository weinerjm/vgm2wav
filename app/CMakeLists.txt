# Rules for building the demo.  Doesn't use an installed gme if you've already
# installed the project so if you're copying these rules you probably don't
# even need these next two lines if you're building against system-installed
# gme.
add_executable(vgm2wav wave_writer.c main.c)
set(GME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libgme/gme
        CACHE PATH "gme include directory")
target_link_libraries(vgm2wav gme)
target_include_directories(vgm2wav PUBLIC ../libgme)
install(FILES vgm2wav DESTINATION /usr/local/bin)
