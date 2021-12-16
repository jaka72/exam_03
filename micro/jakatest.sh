gcc -Wall -Werror -Wextra micro.c && ./a.out map > out
gcc our_micro_paint.c && ./a.out map > outc
diff outc out