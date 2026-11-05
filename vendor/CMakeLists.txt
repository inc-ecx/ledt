set(SDL_TEST OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

add_subdirectory(SDL)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(SDL_ttf)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE SDL3::SDL3)
target_link_libraries(vendor INTERFACE SDL3_ttf::SDL3_ttf)