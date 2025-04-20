CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT(pysol-cards-in-c)

INCLUDE ("${CMAKE_SOURCE_DIR}/cmake/shlomif_common_bootstrap.cmake")
SHLOMIF_COMMON_SETUP("${private_mod_path}")
INCLUDE(GNUInstallDirs)

FIND_PACKAGE(Python3 COMPONENTS Development Development.Embed)

IF( NOT "${Python3_Development.Embed_FOUND}" )
    MESSAGE( WARNING "Python_Development Embed not found" )
ENDIF()
