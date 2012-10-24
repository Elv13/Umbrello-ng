project(umbrello)

cmake_minimum_required(VERSION 2.8)

find_package(KDE4 REQUIRED)

kde4_no_enable_final( umbrello )

add_subdirectory( umbrello ) 

