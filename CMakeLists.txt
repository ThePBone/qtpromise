cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

# metadata
set(META_PROJECT_NAME qtpromise)
set(META_PROJECT_TYPE library)
set(META_APP_NAME "qtpromise")
set(META_PROJECT_VARNAME_UPPER QTPROMISE)
#set(META_PUBLIC_QT_MODULES Gui Widgets Network)
set(META_PROJECT_LICENSE "MIT")

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/qtpromise/*.h)

# add project files
set(HEADER_FILES
    include/QtPromise
    ${SRC_FILES}
)

set(SRC_FILES src/qtpromise/qpromise.inl)


# find c++utilities
find_package(${PACKAGE_NAMESPACE_PREFIX}c++utilities${CONFIGURATION_PACKAGE_SUFFIX} 5.0.0 REQUIRED)
use_cpp_utilities()

# find qtutilities (only CMake modules used)
find_package(${PACKAGE_NAMESPACE_PREFIX}qtutilities${CONFIGURATION_PACKAGE_SUFFIX_QTUTILITIES} 6.3.0 REQUIRED)
use_qt_utilities()

add_definitions(
    -DQT_DEPRECATED_WARNINGS
    -DQT_NO_KEYWORDS
)

# https://github.com/simonbrunel/qtpromise/issues/10
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
    add_compile_options(
        -Werror
        -Wpedantic
        -Wall
        -Wextra
        -Wconversion
        -Wdouble-promotion
        -Wformat=2
        -Wlogical-op
        -Wmissing-noreturn
        -Wold-style-cast
#        -Wshadow                   # disabled due to many findings in the current code
        -Wsign-conversion
        -Wswitch-default
        -Wunused-local-typedefs
        -pedantic-errors
    )

    # https://github.com/Barro/compiler-warnings/blob/master/gcc/warnings-gcc-6.txt
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 6)
        add_compile_options(
            -Wduplicated-cond
        )
    endif()

    # https://github.com/Barro/compiler-warnings/blob/master/gcc/warnings-gcc-7.txt
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7)
        add_compile_options(
            -Wduplicated-branches
        )
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # https://clang.llvm.org/docs/DiagnosticsReference.html
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wsuggest-destructor-override
        -Wsuggest-override
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # https://docs.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level
    add_compile_options(
        /WX
    )
endif()

# link also explicitly against the following Qt modules
#list(APPEND ADDITIONAL_QT_MODULES Network Gui Widgets Svg)

# include modules to apply configuration
include(BasicConfig)
include(QtConfig)
include(LibraryTarget)
include(ConfigHeader)

