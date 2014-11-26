cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

add_subdirectory(FCbin)

add_custom_target(Faraday DEPENDS FaradayCup)
