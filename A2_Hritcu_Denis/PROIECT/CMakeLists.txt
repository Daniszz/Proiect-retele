cmake_minimum_required(VERSION 3.10)
project(ClientPrincipal C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(ClientPrincipal ClientPrincipal.c)
target_include_directories(ClientPrincipal PRIVATE ${GTK3_INCLUDE_DIRS})
target_link_libraries(ClientPrincipal PRIVATE ${GTK3_LIBRARIES})

cmake_minimum_required(VERSION 3.10)
project(ClientSecundar C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(ClientSecundar ClientSecundar.c)
target_include_directories(ClientSecundar PRIVATE ${GTK3_INCLUDE_DIRS})
target_link_libraries(ClientSecundar PRIVATE ${GTK3_LIBRARIES})

