# Rewrites symbolic GPR names ($t0, $ra, ...) to their numeric o32 values, leaving string
# literals alone. Numeric names mean the same thing to every GNU as, regardless of how it maps
# $t0-$t7 under -mabi=eabi. Usage: perl -p tools/regs_numeric.pl < in.s > out.s
BEGIN {
    %m = (zero=>0, at=>1, v0=>2, v1=>3, a0=>4, a1=>5, a2=>6, a3=>7,
          t0=>8, t1=>9, t2=>10, t3=>11, t4=>12, t5=>13, t6=>14, t7=>15,
          s0=>16, s1=>17, s2=>18, s3=>19, s4=>20, s5=>21, s6=>22, s7=>23,
          t8=>24, t9=>25, k0=>26, k1=>27, gp=>28, sp=>29, fp=>30, ra=>31);
}
s/"(?:[^"\\]|\\.)*"(*SKIP)(*FAIL)|\$(zero|at|v[01]|a[0-3]|t[0-9]|s[0-7]|fp|k[01]|gp|sp|ra)(?![0-9A-Za-z_])/"\$$m{$1}"/ge;
