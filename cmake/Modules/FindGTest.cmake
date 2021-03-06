
# Module to find GTest include directory and libraries.

# Module defines
# GTEST_INCLUDE_DIR found by gtest/gtest.h
# GTEST_LIBRARY found by gtest
# GTEST_MAIN_LIBRARY found by gtest_main
# GTEST_FLAG if everything was found

# Module uses
# GTEST_ROOT environment variable as one of the hints

MACRO(DBG_MSG _LINE _MSG)
  MESSAGE(STATUS "${CMAKE_CURRENT_LIST_FILE}(${_LINE}): ${_MSG}")
ENDMACRO(DBG_MSG)

UNSET(GTEST_INCLUDE_DIR CACHE)
FIND_PATH(GTEST_INCLUDE_DIR gtest/gtest.h
	HINTS /usr/include/ /usr/local/include $ENV{GTEST_ROOT}/include
)
IF(GTEST_INCLUDE_DIR)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "GTEST_INCLUDE_DIR ${GTEST_INCLUDE_DIR}")
ELSE(GTEST_INCLUDE_DIR) 
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "Failed to find gtest/gtest.h")
ENDIF(GTEST_INCLUDE_DIR)

UNSET(GTEST_LIBRARY CACHE)
FIND_LIBRARY(GTEST_LIBRARY gtest
	HINTS /usr/lib /usr/local/lib $ENV{GTEST_ROOT}/lib
)
IF(GTEST_LIBRARY)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "GTEST_LIBRARY ${GTEST_LIBRARY}")
ELSE(GTEST_LIBRARY)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "Failed to find lib gtest")
ENDIF(GTEST_LIBRARY)

UNSET(GTEST_MAIN_LIBRARY CACHE)
FIND_LIBRARY(GTEST_MAIN_LIBRARY gtest_main
	HINTS /usr/lib /usr/local/lib $ENV{GTEST_ROOT}/lib
)
IF(GTEST_MAIN_LIBRARY)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "GTEST_MAIN_LIBRARY ${GTEST_MAIN_LIBRARY}")
ELSE(GTEST_MAIN_LIBRARY)
	DBG_MSG(${CMAKE_CURRENT_LIST_LINE} "Failed to find lib gtest_main")
ENDIF(GTEST_MAIN_LIBRARY)

UNSET(GTEST_FLAG CACHE)
IF(GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GTEST_MAIN_LIBRARY)
	MESSAGE(STATUS "Found all required GTest stuff, setting GTEST_FLAG to true =]")
	SET(GTEST_FLAG true)
ELSE(GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GTEST_MAIN_LIBRARY)
	MESSAGE(STATUS "WARNING: Failed to find all or some of the required GTest stuff =[")
ENDIF(GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GTEST_MAIN_LIBRARY)

