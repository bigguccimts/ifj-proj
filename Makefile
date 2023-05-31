# Implementace překladače imperativního jazyka IFJ22 
#
# Matúš Ďurica (xduric06)

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra 
RM=rm -f
PROJ=ifj22

all: *.c clean
	$(CC) $(CFLAGS) -o $(PROJ)  *.c 

clean:
	$(RM) *.o *.out *.ifjcode $(PROJ)

debug: all
	@ cat pokus.txt | ./$(PROJ) > $(PROJ).ifjcode
	@./ic22int $(PROJ).ifjcode
	
val: all
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROJ)<pokus.txt
