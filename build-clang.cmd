@ECHO OFF

clang src\*.c src\mz\*.c src\elf\*.c -fpack-struct=1 -O3 -ogreat.exe