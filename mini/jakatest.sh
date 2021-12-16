gcc -Wall -Werror -Wextra mini.c && ./a.out map > out
gcc our_mini_paint.c && ./a.out map > outc
diff outc out