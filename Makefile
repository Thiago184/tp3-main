all: main.o memoria.o cpu.o hd.o
	@gcc main.o memoria.o cpu.o hd.o -o exe
	@rm -r main.o memoria.o cpu.o hd.o

main.o: main.c
	@gcc main.c -c -Wall

memoria.o: memoria.c
	@gcc memoria.c -c -Wall

cpu.o: cpu.c
	@gcc cpu.c -c -Wall

hd.o: hd.c
	@gcc hd.c -c -Wall

val:
	@gcc -g -o exeval cpu.c memoria.c hd.c main.c -Wall

valrun: val
	@valgrind --leak-check=yes --track-origins=yes -s ./exeval instructions2.txt

run:
	@./exe
