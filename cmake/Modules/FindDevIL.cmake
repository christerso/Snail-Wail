
# This module locates the developer's image library.
# http://openil.sourceforge.net/
#
# This module sets:
# IL_LIBRARY the name of the IL library.
# ILU_LIBRARY the name of the ILU library.
# ILUT_LIBRARY the name of the ILUT library.
# IL_INCLUDE_DIR where to find the il.h, ilu.h and ilut.h files.
# IL_FOUND this is set to TRUE if all the above variables were set.

# Original file by: Christopher Harvey
# TODO: Add version support.
# Tested under Linux and Windows (MSVC)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)

FIND_PATH(IL_INCLUDE_DIR il.h 
  PATH_SUFFIXES include IL
  DOC "The path the the directory that contains il.h"
  PATHS
  ${PROJECT_ROOT_DIR}/3pp/include/ # added by ptr
)

#MESSAGE("IL_INCLUDE_DIR is ${IL_INCLUDE_DIR}")

FIND_LIBRARY(IL_LIBRARY
  NAMES IL DevIL
  PATH_SUFFIXES lib64 lib lib32
  DOC "The file that corresponds to the base il library."
  PATHS
  ${PROJECT_ROOT_DIR}/3pp/lib/ # added by ptr
)

#MESSAGE("IL_LIBRARY is ${IL_LIBRARY}")

FIND_PACKAGE_HANDLE_STANDARD_ARGS(IL DEFAULT_MSG 
                                  IL_LIBRARY IL_INCLUDE_DIR)