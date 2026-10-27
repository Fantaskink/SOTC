# Shadow of the Colossus decompilation,
#
#   cmake --preset default && cmake --build out && ctest --test-dir out
#
# Files:  cmake/Dependencies.cmake  venv, compiler download, tool and game-file checks
#         cmake/Toolchain.cmake     assembler, python, patcher settings and quirks
#         cmake/Decomp.cmake        workflow: objects -> link -> objcopy -> sha1 test
#
# Options (-D<NAME>=...): SOTC_MODULES, DECOMP_AUTO_SETUP, DECOMP_CROSS, DECOMP_CC, DECOMP_PYTHON,
#                         DECOMP_NUMERIC_REGS, DECOMP_EEGCC_URL, DECOMP_EEGCC_SHA256

cmake_minimum_required(VERSION 3.20)
project(sotc LANGUAGES NONE)

# The modules come from config/modules.txt (name, file, format, ...), shared with configure.py and make.
set(_modules_file "${CMAKE_SOURCE_DIR}/config/modules.txt")
set_property(
        DIRECTORY
        APPEND
        PROPERTY CMAKE_CONFIGURE_DEPENDS "${_modules_file}"
)
file(STRINGS "${_modules_file}" _lines REGEX "^[ \t]*[^ \t#]") # skips blank and comment lines
set(SOTC_ALL_MODULES "")
foreach (_line IN LISTS _lines)
    string(REGEX REPLACE "#.*$" "" _line "${_line}")
    string(REGEX REPLACE "[ \t]+" ";" _fields "${_line}")

    list(LENGTH _fields _count)
    if (_count LESS 3)
        message(FATAL_ERROR "${_modules_file}: expected '<name> <file> <elf|xff> ...', got '${_line}'")
    endif ()

    list(GET _fields 0 _name)
    list(GET _fields 1 _base)
    list(APPEND SOTC_ALL_MODULES "${_name}:${_base}")
endforeach ()

set(SOTC_MODULES
        "all"
        CACHE STRING "modules to extract and build: 'all', or a list like 'loader' or 'loader;kernel'"
)

set(SOTC_ENABLED "")
set(SOTC_ENABLED_BASENAMES "")
set(_known "")
foreach (_entry IN LISTS SOTC_ALL_MODULES)
    string(REPLACE ":" ";" _pair "${_entry}")
    list(GET _pair 0 _name)
    list(GET _pair 1 _base)
    list(APPEND _known ${_name})

    if (SOTC_MODULES STREQUAL "all" OR _name IN_LIST SOTC_MODULES)
        list(APPEND SOTC_ENABLED "${_entry}")
        list(APPEND SOTC_ENABLED_BASENAMES ${_base})
    endif ()

endforeach ()
foreach (_m IN LISTS SOTC_MODULES) # a typo should not silently build nothing
    if (NOT _m STREQUAL "all" AND NOT _m IN_LIST _known)
        message(FATAL_ERROR "Unknown module '${_m}'. Known modules: ${_known}")
    endif ()

endforeach ()

if (NOT SOTC_ENABLED)
    message(FATAL_ERROR "SOTC_MODULES='${SOTC_MODULES}' selects no module. Known modules: ${_known}")
endif ()

enable_testing() # before the modules, or the sha1 tests are not registered

include(cmake/Dependencies.cmake)
decomp_check_dependencies() # venv, compiler, tools, game files
include(cmake/Toolchain.cmake) # assembler, python, patcher settings
include(cmake/Decomp.cmake) # the build rules

foreach (_entry IN LISTS SOTC_ENABLED)
    string(REPLACE ":" ";" _pair "${_entry}")
    list(GET _pair 0 _name)
    list(GET _pair 1 _base)
    decomp_add_module(${_name} BASENAME ${_base})
endforeach ()
