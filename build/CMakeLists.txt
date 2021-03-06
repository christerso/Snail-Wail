# -*- mode: cmake -*-

cmake_minimum_required (VERSION 2.4)
project (SnailWail)

include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

#CHECK_INCLUDE_FILE_CXX("tr1/memory" HAVE_TR1_MEMORY_H)

# check which smart pointers we can use (std, std::tr1, boost, ...)

SET(VERIFY_SMARTPTR_STD
  "class A {public: virtual ~A() {} }; class B : public A {}; int main() {std::shared_ptr<A> ptr1; std::weak_ptr<A> ptr2(ptr1); std::dynamic_pointer_cast<B>(ptr1); }")
SET(VERIFY_SMARTPTR_TR1
  "class A {public: virtual ~A() {} }; class B : public A {}; int main() {std::tr1::shared_ptr<A> ptr1; std::tr1::weak_ptr<A> ptr2(ptr1); std::tr1::dynamic_pointer_cast<B>(ptr1); }")
SET(VERIFY_SMARTPTR_BOOST
  "class A {public: virtual ~A() {} }; class B : public A {}; int main() {boost::shared_ptr<A> ptr1; boost::weak_ptr<A> ptr2(ptr1); boost::dynamic_pointer_cast<B>(ptr1); }")

# TODO: only one VERIFY_SMARTPTR, send parameter for namespace
CHECK_CXX_SOURCE_COMPILES("#include <memory>\n ${VERIFY_SMARTPTR_STD}" HAVE_STD_SMARTPTRS)

if (NOT HAVE_STD_SMARTPTRS)
  CHECK_CXX_SOURCE_COMPILES("#include <tr1/memory>\n ${VERIFY_SMARTPTR_TR1}" HAVE_TR1_SMARTPTRS)

  if (NOT HAVE_TR1_SMARTPTRS)
	# check if the standard include file <memory> contains tr1; this is what MSVC does
	CHECK_CXX_SOURCE_COMPILES("#include <memory>\n ${VERIFY_SMARTPTR_TR1}" HAVE_TR1_IN_STD_SMARTPTRS)

	if (NOT HAVE_TR1_IN_STD_SMARTPTRS)
	  # no smart pointer support in either tr1 nor sc++l, fall back to boost
	  find_package(Boost REQUIRED)
	  if (Boost_FOUND)
		SET(HAVE_BOOST 1)

		# really verify that boost provides this
		CHECK_CXX_SOURCE_COMPILES("#include <boost/shared_ptr.hpp>\n#include <boost/weak_ptr.hpp>\n ${VERIFY_SMARTPTR_BOOST}" HAVE_BOOST_SMARTPTRS)
	  endif (Boost_FOUND)
	endif (NOT HAVE_TR1_IN_STD_SMARTPTRS)
  endif (NOT HAVE_TR1_SMARTPTRS)
endif (NOT HAVE_STD_SMARTPTRS)

ADD_DEFINITIONS(-g)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)

CONFIGURE_FILE(${PROJECT_ROOT_DIR}/src/config.h.in ${PROJECT_ROOT_DIR}/src/config.h)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0011 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_MODULE_PATH "${PROJECT_ROOT_DIR}/cmake/Modules/")

include_directories ("${PROJECT_ROOT_DIR}/inc/" "${PROJECT_ROOT_DIR}/src/")

find_package(GTest)

add_subdirectory (${PROJECT_ROOT_DIR}/src CMakeFiles/Src)
add_subdirectory (${PROJECT_ROOT_DIR}/data CMakeFiles/Data)

add_custom_target(clean-gen
	rm -rf CMakeFiles CMakeCache.txt Makefile cmake_install.cmake ../bin 
	install_manifest.txt _CPack_Packages *.deb CPack*	
)

# Invoke CPack for Debian
include(UseDebian)
