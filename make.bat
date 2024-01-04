nasm -f win64 arithmetics.asm -o arithmetics.o
gcc main.c arithmetics.o -o main.exe