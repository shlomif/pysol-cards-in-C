CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT(pysol-cards-in-c)

INCLUDE ("${CMAKE_SOURCE_DIR}/cmake/shlomif_common_bootstrap.cmake")
SHLOMIF_COMMON_SETUP("${private_mod_path}")
INCLUDE(GNUInstallDirs)

FIND_PACKAGE(Python3 COMPONENTS Development Development.Embed)

IF( NOT "${Python3_Development.Embed_FOUND}" )
    MESSAGE( FATAL_ERROR "Python_Development Embed not found" )
ENDIF()

SET(wrapperexe pysol_cards_wrap)
SET(wrappermod "call.c")
ADD_EXECUTABLE("${wrapperexe}" "${wrappermod}")
SET_TARGET_PROPERTIES("${wrapperexe}" PROPERTIES INCLUDE_DIRECTORIES ${Python3_INCLUDE_DIRS})
if()
# SET_TARGET_PROPERTIES("${wrapperexe}" PROPERTIES INCLUDE_DIRECTORIES "${Python3_Development.Embed_INCLUDE_DIRS}")
SET_TARGET_PROPERTIES("${wrapperexe}" PROPERTIES LIBRARIES "${Python3_LIBRARIES};${Python3_Development.Embed_LIBRARIES}")
SET_TARGET_PROPERTIES("${wrapperexe}" PROPERTIES LINK_OPTIONS "${Python3_LINK_OPTIONS} ${Python3_Development.Embed_LINK_OPTIONS}")
endif()
SET_TARGET_PROPERTIES("${wrapperexe}" PROPERTIES LIBRARIES "${Python3_Development.Embed_LIBRARIES}")
SET_TARGET_PROPERTIES("${wrapperexe}" PROPERTIES LINK_OPTIONS "${Python3_Development.Embed_LINK_OPTIONS};${Python3_LIBRARY_RELEASE}")
