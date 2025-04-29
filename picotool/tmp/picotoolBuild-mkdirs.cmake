# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/gulbaran/wifi/_deps/picotool-src"
  "/home/gulbaran/wifi/_deps/picotool-build"
  "/home/gulbaran/wifi/_deps"
  "/home/gulbaran/wifi/picotool/tmp"
  "/home/gulbaran/wifi/picotool/src/picotoolBuild-stamp"
  "/home/gulbaran/wifi/picotool/src"
  "/home/gulbaran/wifi/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/gulbaran/wifi/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/gulbaran/wifi/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
