# usage: cmake -DFILE=<binary> -DSHA1FILE=<checksum-*.sha1> -P CheckSha1.cmake
file(STRINGS "${SHA1FILE}" line LIMIT_COUNT 1)
string(REGEX MATCH "^[0-9a-fA-F]+" want "${line}")
file(SHA1 "${FILE}" got)
if (NOT got STREQUAL want)
    message(FATAL_ERROR "sha1 mismatch for ${FILE}\n  got:  ${got}\n  want: ${want}")
endif ()
