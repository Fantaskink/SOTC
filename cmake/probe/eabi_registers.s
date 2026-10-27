# Assembled by cmake/Toolchain.cmake to learn which register names this GNU as accepts under
# -mabi=eabi. $t4 exists in the o32-style names the project's asm is written in; newer GNU as
# snapshots apply n32-style names to eabi, where $t4 does not exist.
.set noat
addu $v1,$v1,$t4
