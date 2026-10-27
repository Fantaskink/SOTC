# Build rules for the SOTC decomp.
#
#   configure_o.py --emit-cmake   writes <build>/<module>/objects.cmake, one decomp_object() per object
#               |
#   decomp_add_module()         includes it (each decomp_object() defines that object's rule), then
#               |               links with splat's .ld, runs objcopy (+ fix_xff.py for the kernel),
#               v               and registers the sha1 check as a test
#   cmake --build / ctest
#
# Needs the Ninja generator (it creates output directories) and cmake/Toolchain.cmake included first.
# Commands run from the repo root, so relative paths match the legacy ninja rules.

# <src> and <obj> are repo-relative as the manifest gives them (they go into the command line);
# <abs_src> and <out> are absolute (Ninja needs those for dependencies and outputs).
function(_decomp_compile_rule out obj src abs_src optlevel regnames)
    separate_arguments(include_flags UNIX_COMMAND "${DECOMP_INCLUDES}")
    separate_arguments(opt_flags UNIX_COMMAND "${optlevel}")
    set(small_data_flags "")

    if (DECOMP_XFF)
        set(small_data_flags -G0) # the kernel's legacy compile rule has it, the loader's doesn't
    endif ()

    add_custom_command(
            OUTPUT "${out}"
            COMMAND "${DECOMP_CC}" -c ${include_flags} ${opt_flags} ${small_data_flags} -g2 ${regnames} "${src}" -o "${obj}"
            COMMAND "${DECOMP_CROSS}strip" "${obj}" -N dummy-symbol-name
            DEPENDS "${abs_src}"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            VERBATIM
    )
endfunction()

# The shell pipeline gets the source as $1, the object as $2 and the section-align override as $3.
function(_decomp_assemble_rule out obj src abs_src override)
    set(stages "")
    set(extra_deps "")

    if (DECOMP_XFF)
        # The legacy rule is `cpp | iconv -f=UTF-8 -t=EUC-JP $in | as`. iconv reads $in itself and
        # ignores stdin, so cpp's output was never used; leaving cpp out gives identical objects.
        list(APPEND stages [[iconv -f UTF-8 -t EUC-JP "$1"]])
    else ()
        string(CONFIGURE [[cpp @DECOMP_INCLUDES@ "$1"]] preprocess @ONLY)
        list(APPEND stages "${preprocess}")
    endif ()

    if (DECOMP_REGFILTER)
        string(CONFIGURE [["@DECOMP_PERL@" -p tools/regs_numeric.pl]] regs_filter @ONLY)
        list(APPEND stages "${regs_filter}")
        list(APPEND extra_deps "${CMAKE_SOURCE_DIR}/tools/regs_numeric.pl")
    endif ()

    string(CONFIGURE [[@DECOMP_CROSS@as -no-pad-sections -EL -march=5900 -mabi=eabi -Iinclude -o "$2"]] assembler @ONLY)
    list(APPEND stages "${assembler}")
    list(JOIN stages " | " script)

    if (NOT DECOMP_XFF)
        string(CONFIGURE [[ && "@DECOMP_PYTHON@" @DECOMP_PATCHER@ "$2" gas $3]] patch_alignment @ONLY)
        string(APPEND script "${patch_alignment}")
    endif ()

    add_custom_command(
            OUTPUT "${out}"
            COMMAND sh -c "${script}" decomp-as "${src}" "${obj}" "${override}"
            DEPENDS "${abs_src}" ${extra_deps}
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            VERBATIM
    )
endfunction()

# decomp_object(<cc|as> <object> SRC <source> [OPTLEVEL ..] [REGNAMES ..] [OVERRIDE ..])
# Called from the manifest, which also sets DECOMP_XFF and DECOMP_INCLUDES.
function(decomp_object kind obj)
    cmake_parse_arguments(ARG "" "OPTLEVEL;REGNAMES;OVERRIDE" "SRC" ${ARGN})

    list(LENGTH ARG_SRC source_count)
    if (NOT source_count EQUAL 1)
        message(FATAL_ERROR "decomp_object(${obj}): expected exactly one SRC, got ${source_count}")
    endif ()

    cmake_path(ABSOLUTE_PATH obj BASE_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE out)
    cmake_path(ABSOLUTE_PATH ARG_SRC BASE_DIRECTORY "${CMAKE_SOURCE_DIR}" OUTPUT_VARIABLE abs_src)

    if (kind STREQUAL "cc")
        _decomp_compile_rule("${out}" "${obj}" "${ARG_SRC}" "${abs_src}" "${ARG_OPTLEVEL}" "${ARG_REGNAMES}")
    elseif (kind STREQUAL "as")
        _decomp_assemble_rule("${out}" "${obj}" "${ARG_SRC}" "${abs_src}" "${ARG_OVERRIDE}")
    else ()
        message(FATAL_ERROR "decomp_object(${obj}): rule '${kind}' is not defined (ninja has none either)")
    endif ()

    list(APPEND DECOMP_OBJS "${out}")
    set(DECOMP_OBJS
            "${DECOMP_OBJS}"
            PARENT_SCOPE # collected by decomp_add_module
    )
endfunction()

# Runs splat through configure_o.py, but only when something it reads changed, not on every CMake edit.
function(_decomp_write_manifest name base manifest)
    set(yaml "${CMAKE_SOURCE_DIR}/config/${base}/${base}.yaml")
    set(script "${CMAKE_SOURCE_DIR}/configure.py")
    set_property(
            DIRECTORY
            APPEND
            PROPERTY CMAKE_CONFIGURE_DEPENDS "${yaml}" "${script}")

    if (NOT EXISTS "${manifest}"
            OR NOT EXISTS "${CMAKE_SOURCE_DIR}/asm/${base}"
            OR "${yaml}" IS_NEWER_THAN "${manifest}"
            OR "${script}" IS_NEWER_THAN "${manifest}"
    )
        set(ENV{DECOMP_PYTHON} "${DECOMP_PYTHON}")
        set(ENV{DECOMP_CC} "${DECOMP_CC}")

        message(STATUS "decomp: extracting ${base} with splat")
        execute_process(COMMAND "${DECOMP_PYTHON}" "${script}" -c -t ${name} --emit-cmake "${manifest}"
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                COMMAND_ERROR_IS_FATAL ANY
        )
    endif ()
endfunction()

# decomp_add_module(<target name> BASENAME <name of the file in iso/>)
function(decomp_add_module name)
    cmake_parse_arguments(ARG "" "BASENAME" "" ${ARGN})
    set(base "${ARG_BASENAME}")
    set(root "${CMAKE_SOURCE_DIR}")

    # Same paths the legacy build uses, so asm-differ, mapfile tools etc. keep working.
    set(manifest "${CMAKE_BINARY_DIR}/${name}/objects.cmake")
    set(linker_script "${base}.ld") # written by splat
    set(linked_elf "build/${base}/${base}.elf")
    set(binary "build/${base}/${base}")
    set(map_file "build/${base}/${base}.map")
    set(symbol_scripts config/${base}/${base}_undefined_syms_auto.txt config/${base}/${base}_undefined_funcs_auto.txt
            config/${base}/undefined_syms.txt
    )

    # 1. extract, then include the manifest: it defines the rule for every object
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${name}")
    _decomp_write_manifest(${name} ${base} "${manifest}")
    set(DECOMP_OBJS "")
    include("${manifest}") # sets DECOMP_XFF and DECOMP_INCLUDES, calls decomp_object()

    # 2. link: the objects are named inside splat's linker script, so they are only dependencies
    set(link_flags -EL)
    set(link_deps "${root}/${linker_script}")

    foreach (script IN LISTS symbol_scripts)
        list(APPEND link_flags -T "${script}")

        if (EXISTS "${root}/${script}")
            list(APPEND link_deps "${root}/${script}")
        endif ()

    endforeach ()

    list(APPEND link_flags -Map "${map_file}")

    if (DECOMP_XFF)
        list(APPEND link_flags -r) # the kernel stays relocatable
    endif ()

    add_custom_command(
            OUTPUT "${root}/${linked_elf}"
            COMMAND "${DECOMP_CROSS}ld" ${link_flags} -T "${linker_script}" -o "${linked_elf}"
            DEPENDS ${DECOMP_OBJS} ${link_deps}
            WORKING_DIRECTORY "${root}"
            VERBATIM
    )

    # 3. flat binary (the kernel additionally needs its XFF header fixed up)
    set(fix_xff "")

    if (DECOMP_XFF)
        set(fix_xff COMMAND "${DECOMP_PYTHON}" tools/fix_xff.py "${binary}")
    endif ()

    add_custom_command(
            OUTPUT "${root}/${binary}"
            COMMAND "${DECOMP_CROSS}objcopy" "${linked_elf}" "${binary}" -O binary ${fix_xff}
            DEPENDS "${root}/${linked_elf}"
            WORKING_DIRECTORY "${root}"
            VERBATIM
    )

    add_custom_target(${name} ALL DEPENDS "${root}/${binary}")

    # 4. does the result hash to the original game file?
    add_test(NAME ${name}_sha1 COMMAND "${CMAKE_COMMAND}" -DFILE=${root}/${binary}
            -DSHA1FILE=${root}/checksum-${base}.sha1 -P "${root}/cmake/CheckSha1.cmake"
    )
endfunction()
