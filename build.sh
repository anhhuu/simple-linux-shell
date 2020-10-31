rm build/history.o
rm build/execution.o
rm build/parsing.o
rm build/utils.o
rm build/main.o
rm bin/osh

mkdir bin
mkdir build

gcc src/history.c -o build/history.o -c
gcc src/execution.c -o build/execution.o -c
gcc src/parsing.c -o build/parsing.o -c
gcc src/utils.c -o build/utils.o -c
gcc src/main.c -o build/main.o -c

gcc -o bin/osh build/history.o build/execution.o build/parsing.o build/utils.o build/main.o