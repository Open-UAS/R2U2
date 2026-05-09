# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 4.0)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(r2u2-espidf)
# idk if this is permanent but the r2u2 library gives a couple of warnings I would like to ignore
# bc idk what I'm doing
idf_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)