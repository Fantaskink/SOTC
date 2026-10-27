# Checks, and unless -DDECOMP_AUTO_SETUP=OFF sets up, what the build needs. Call
# decomp_check_dependencies() once, before including cmake/Toolchain.cmake.
#
#   venv      .python3 with splat, spimdisasm, ...            created and pip-installed when missing
#   compiler  tools/cc/ee-gcc2.96 (what setup_compiler.sh did) downloaded, verified and extracted
#   tools     cpp, iconv, sh                                    checked only
#   game      iso/<module file> per enabled module              checked only, extract them from your ISO
#
# Not done here on purpose: installing system packages (needs root, differs per distro).
#   -DDECOMP_EEGCC_SHA256=  (empty) skips verifying the compiler archive

option(DECOMP_AUTO_SETUP "Create the python venv and download the compiler when they are missing" ON)
set(DECOMP_EEGCC_URL
        "https://github.com/decompme/compilers/releases/download/compilers/ee-gcc2.96.tar.xz"
        CACHE STRING "where to download the ee-gcc 2.96 archive from"
)
set(DECOMP_EEGCC_SHA256
        "0590d2ca9da8f5903889d66761220d14b47a8d14ba987ca53db84a1650a1fd0a"
        CACHE STRING "expected SHA-256 of that archive (empty = do not verify)"
)
set(DECOMP_PYTHON CACHE STRING "${CMAKE_SOURCE_DIR}/.python3/bin/python3")
set(DECOMP_CC CACHE STRING "${CMAKE_SOURCE_DIR}/tools/cc/ee-gcc2.96/bin/ee-gcc")
set(ISO_PATH CACHE STRING "${CMAKE_SOURCE_DIR}/iso")

function(_decomp_ensure_python_venv)
    get_filename_component(_python_dir "${DECOMP_PYTHON}" DIRECTORY)
    get_filename_component(venv "${_python_dir}" DIRECTORY)
    set(requirements "${CMAKE_SOURCE_DIR}/requirements.txt")
    set(stamp "${venv}/.requirements.sha256") # hash of the requirements pip last installed

    set_property(
            DIRECTORY
            APPEND
            PROPERTY CMAKE_CONFIGURE_DEPENDS "${requirements}"
    )

    file(SHA256 "${requirements}" wanted_hash)

    if (DECOMP_AUTO_SETUP)
        if (NOT EXISTS "${DECOMP_PYTHON}")
            find_program(system_python NAMES python3 python REQUIRED)
            message(STATUS "decomp: creating the python venv in ${venv}")

            execute_process(
                    COMMAND "${system_python}" -m venv "${venv}"
                    RESULT_VARIABLE rc
                    ERROR_VARIABLE err
            )

            if (NOT rc EQUAL 0)
                file(REMOVE_RECURSE "${venv}")
                message(FATAL_ERROR "could not create ${venv}:\n${err}\n"
                        "On Debian/Ubuntu: sudo apt install python3-venv python3-pip"
                )
            endif ()

        endif ()

        set(installed_hash "")
        if (EXISTS "${stamp}")
            file(READ "${stamp}" installed_hash)
            string(STRIP "${installed_hash}" installed_hash)
        endif ()

        if (NOT installed_hash STREQUAL wanted_hash)
            message(STATUS "decomp: installing python requirements (requirements.txt changed or first run)")
            execute_process(COMMAND "${DECOMP_PYTHON}" -m pip install -U -r "${requirements}" RESULT_VARIABLE rc)

            if (NOT rc EQUAL 0)
                message(FATAL_ERROR "pip install -r requirements.txt failed (see the output above)")
            endif ()

            file(WRITE "${stamp}" "${wanted_hash}\n")
        endif ()

    endif ()

    if (NOT EXISTS "${DECOMP_PYTHON}")
        message(FATAL_ERROR "${DECOMP_PYTHON} is missing. Re-run with -DDECOMP_AUTO_SETUP=ON or create it as the README says.")
    endif ()

endfunction()


function(_decomp_ensure_compiler)
    get_filename_component(_compiler_bin "${DECOMP_CC}" DIRECTORY) #
    get_filename_component(_compiler_root "${DECOMP_CC}" DIRECTORY)

    if (NOT EXISTS "${DECOMP_CC}")
        if (NOT DECOMP_AUTO_SETUP)
            message(FATAL_ERROR "${DECOMP_CC} is missing; run scripts/setup_compiler.sh "
                    "or use -DDECOMP_AUTO_SETUP=ON"
            )
        endif ()

        set(archive "${CMAKE_BINARY_DIR}/ee-gcc2.96.tar.xz")
        set(verify "")
        if (DECOMP_EEGCC_SHA256)
            set(verify EXPECTED_HASH SHA256=${DECOMP_EEGCC_SHA256})
        endif ()

        message(STATUS "decomp: downloading ${DECOMP_EEGCC_URL}")
        file(
                DOWNLOAD "${DECOMP_EEGCC_URL}" "${archive}"
                ${verify}
                STATUS download
                TLS_VERIFY ON
        )

        list(GET download 0 download_code)
        if (NOT download_code EQUAL 0)
            file(REMOVE "${archive}")
            message(
                    FATAL_ERROR
                    "downloading the compiler failed: ${download}\n"
                    "If the hash changed upstream, check the release and update DECOMP_EEGCC_SHA256 "
                    "(or pass -DDECOMP_EEGCC_SHA256= to skip the check)."
            )
        endif ()

        file(MAKE_DIRECTORY "${DECOMP_CC}")
        file(ARCHIVE_EXTRACT INPUT "${archive}" DESTINATION "${DECOMP_CC}")
        file(
                CHMOD_RECURSE
                "${DECOMP_CC}"
                FILE_PERMISSIONS
                OWNER_READ
                OWNER_WRITE
                OWNER_EXECUTE
                GROUP_READ
                GROUP_EXECUTE
                WORLD_READ
                WORLD_EXECUTE
        )

        file(REMOVE "${archive}")
    endif ()

    # ee-gcc is a 32-bit x86 executable; on a 64-bit host it needs the 32-bit loader and libc
    execute_process(
            COMMAND "${DECOMP_CC}" --version
            RESULT_VARIABLE rc
            OUTPUT_QUIET ERROR_QUIET
    )

    if (NOT rc EQUAL 0)
        message(FATAL_ERROR "${DECOMP_CC} does not run (${rc}). It is a 32-bit program; "
                "on Debian/Ubuntu install the 32-bit libc:  sudo apt install libc6-i386")
    endif ()

endfunction()

function(_decomp_check_tools)
    set(missing "")
    foreach (tool cpp iconv sh)
        find_program(_decomp_tool_${tool} NAMES ${tool})
        if (NOT _decomp_tool_${tool})
            list(APPEND missing ${tool})
        endif ()

    endforeach ()

    if (missing)
        message(FATAL_ERROR "missing tools: ${missing}  (Debian/Ubuntu: sudo apt install cpp libc-bin)")
    endif ()

endfunction()

# The game files cannot be downloaded: SOTC_ENABLED_BASENAMES comes from the root CMakeLists.txt.
function(_decomp_check_game_files)
    set(missing "")
    foreach (basename ${SOTC_ENABLED_BASENAMES})
        if (NOT EXISTS "${CMAKE_SOURCE_DIR}/iso/${basename}")
            list(APPEND missing "iso/${basename}")
        endif ()

    endforeach ()

    if (missing)
        message(FATAL_ERROR "missing game files: ${missing}\n"
                "Extract them from a Shadow of the Colossus Preview ISO into ${CMAKE_SOURCE_DIR}/iso/"
        )
    endif ()

endfunction()

function(decomp_check_dependencies)
    _decomp_ensure_python_venv()
    _decomp_ensure_compiler()
    _decomp_check_tools()
    _decomp_check_game_files()
endfunction()
