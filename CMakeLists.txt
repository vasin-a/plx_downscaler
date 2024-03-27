cmake_minimum_required(VERSION 3.22.1)

project(plx_downscaler)

include(shared.cmake)

add_subdirectory(ext)
add_subdirectory(lib)
add_subdirectory(host)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "host")

include(InstallRequiredSystemLibraries)
install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)
install(TARGETS host CONFIGURATIONS Release RUNTIME DESTINATION bin)
