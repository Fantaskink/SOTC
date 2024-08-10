.macro glabel label
    .global \label
    .type \label, @function
    \label:
.endm

.macro endlabel label
.endm

.macro alabel label
    .global \label
    \label:
.endm

.macro dlabel label
    .global \label
    \label:
.endm

.macro jlabel label
#if VERSION_CN
    .global \label
    .type \label, @function
#endif
    \label:
.endm
