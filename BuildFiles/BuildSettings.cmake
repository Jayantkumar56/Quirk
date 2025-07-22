
# Set global output directories
# CMAKE_BINARY_DIR will be defined by the top-level CMakeLists.txt or build system
set( BIN_BASE_DIR  "${CMAKE_BINARY_DIR}/bin"     )
set( LIB_BASE_DIR  "${CMAKE_BINARY_DIR}/bin"     )
set( ARCH_BASE_DIR "${CMAKE_BINARY_DIR}/archive" )

# For Executables and Shared Libraries (.dll, .so, .dylib)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY                 "${BIN_BASE_DIR}"                )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG           "${BIN_BASE_DIR}/Debug"          )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE         "${BIN_BASE_DIR}/Release"        )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO  "${BIN_BASE_DIR}/RelWithDebInfo" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL      "${BIN_BASE_DIR}/MinSizeRel"     )

# For Static Libraries (.lib, .a)
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY                 "${ARCH_BASE_DIR}"                )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG           "${ARCH_BASE_DIR}/Debug"          )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE         "${ARCH_BASE_DIR}/Release"        )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO  "${ARCH_BASE_DIR}/RelWithDebInfo" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL      "${ARCH_BASE_DIR}/MinSizeRel"     )

# For Module Libraries (if distinct, otherwise runtime often applies)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY                 "${LIB_BASE_DIR}"                )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG           "${LIB_BASE_DIR}/Debug"          )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE         "${LIB_BASE_DIR}/Release"        )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO  "${LIB_BASE_DIR}/RelWithDebInfo" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL      "${LIB_BASE_DIR}/MinSizeRel"     )

# C++ Standard to be used
set( CMAKE_CXX_STANDARD          20  )
set( CMAKE_CXX_STANDARD_REQUIRED ON  )
set( CMAKE_CXX_EXTENSIONS        OFF )

# ensures single-config generators build in Release mode by default
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Set global property for folder organization in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
