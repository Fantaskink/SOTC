# Tool settings and toolchain quirks. Every cache variable can be overridden with -D<NAME>=...
#
#   DECOMP_CROSS         prefix of the MIPS binutils (as, ld, objcopy, strip)  mips-linux-gnu-
#   DECOMP_CC            the game's compiler, ee-gcc 2.96                      tools/cc/ee-gcc2.96/bin/ee-gcc
#   DECOMP_NUMERIC_REGS  AUTO | ON | OFF, see _decomp_choose_register_mode     AUTO
#   DECOMP_PYTHON        python with splat installed                           .python3/bin/python3
#
# Sets for the rest of the build: DECOMP_REGFILTER, DECOMP_PERL (if needed), DECOMP_PATCHER.

set(DECOMP_CROSS
        "mips-linux-gnu-"
        CACHE STRING "prefix of the MIPS binutils"
)
set(DECOMP_CC
        "${CMAKE_SOURCE_DIR}/tools/cc/ee-gcc2.96/bin/ee-gcc"
        CACHE FILEPATH "the game's compiler"
)
set(DECOMP_NUMERIC_REGS
        "AUTO"
        CACHE STRING "rewrite $t0-style register names to numbers before assembling: AUTO, ON or OFF"
)
set_property(CACHE DECOMP_NUMERIC_REGS PROPERTY STRINGS AUTO ON OFF)

set(_DECOMP_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}") # CMAKE_CURRENT_LIST_DIR is the caller's inside functions

function(_decomp_require_assembler)
    execute_process(
            COMMAND "${DECOMP_CROSS}as" --version
            RESULT_VARIABLE rc
            OUTPUT_QUIET ERROR_QUIET
    )

    if (NOT rc EQUAL 0)
        message(FATAL_ERROR "'${DECOMP_CROSS}as' was not found or cannot run. Install binutils-mips-linux-gnu "
                "or pass -DDECOMP_CROSS=/path/to/bin/mips-linux-gnu-"
        )
    endif ()

endfunction()

# result_var = TRUE when the assembler understands o32-style names ($t4, ...) under -mabi=eabi
function(_decomp_assembler_accepts_o32_names result_var)
    execute_process(
            COMMAND "${DECOMP_CROSS}as" -EL -march=5900 -mabi=eabi -o /dev/null "${_DECOMP_CMAKE_DIR}/probe/eabi_registers.s"
            RESULT_VARIABLE rc
            OUTPUT_QUIET ERROR_QUIET
    )

    if (rc EQUAL 0)
        set(${result_var}
                TRUE
                PARENT_SCOPE
        )
    else ()
        set(${result_var}
                FALSE
                PARENT_SCOPE
        )
    endif ()

endfunction()

# Development snapshots of GNU as after the 2.45 release pick n32-style register names for
# -mabi=eabi: $t4-$t7 stop existing and $t0-$t3 silently become $12-$15 instead of $8-$11.
#   AUTO  probe the assembler; only if it behaves that way, rewrite symbolic registers to numbers
#         (same encoding on every version) just before assembling
#   ON    always rewrite          OFF  never rewrite (stops if the assembler needs it)
# Sets DECOMP_REGFILTER to ON/OFF.
function(_decomp_choose_register_mode)
    _decomp_assembler_accepts_o32_names(accepts)

    if (DECOMP_NUMERIC_REGS STREQUAL "AUTO")
        if (accepts)
            set(rewrite OFF)
        else ()
            set(rewrite ON)
            message(STATUS "decomp: '${DECOMP_CROSS}as' rejects $t4 under -mabi=eabi; rewriting registers to numbers")
        endif ()

    elseif (DECOMP_NUMERIC_REGS)
        set(rewrite ON)

    else ()
        set(rewrite OFF)
        if (NOT accepts)
            message(FATAL_ERROR "'${DECOMP_CROSS}as' rejects $t4 under -mabi=eabi (newer GNU as uses n32 register "
                    "names there). Use -DDECOMP_NUMERIC_REGS=AUTO (or ON), or an older binutils."
            )
        endif ()

    endif ()

    if (rewrite)
        find_program(DECOMP_PERL NAMES perl REQUIRED)
    endif ()

    set(DECOMP_REGFILTER
            ${rewrite}
            PARENT_SCOPE
    )
endfunction()

_decomp_require_assembler()
_decomp_choose_register_mode()

# configure.py needs splat & co, which live in the repo's venv (README: ./.python3)
if (NOT DECOMP_PYTHON)
    if (EXISTS "${CMAKE_SOURCE_DIR}/.python3/bin/python3")
        set(DECOMP_PYTHON
                "${CMAKE_SOURCE_DIR}/.python3/bin/python3"
                CACHE FILEPATH "python with splat installed"
        )

    else ()
        find_program(DECOMP_PYTHON NAMES python3 python REQUIRED)
    endif ()

endif ()

# the dependency-free patcher is byte-identical and ~6x faster per object
if (EXISTS "${CMAKE_SOURCE_DIR}/tools/elf_patcher_lite.py")
    set(DECOMP_PATCHER tools/elf_patcher_lite.py)
else ()
    set(DECOMP_PATCHER tools/elf_patcher.py)
endif ()
