# Implementace překladače imperativního jazyka IFJ22 
#
# Matúš Ďurica (xduric06) 
# Ivan Mahút (xmahut01) 
# Dušan Slúka (xsluka00)
# Gabriela Paganíková (xpagan00)

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -Werror
RM=rm -f
PROJ=ifj22

all: *.c clean
	$(CC) $(CFLAGS) -o $(PROJ)  *.c 

clean:
	$(RM) *.o *.out *.ifjcode $(PROJ)

debug: all
	@./$(PROJ) > $(PROJ).ifjcode
	@./ic22int $(PROJ).ifjcode
	
val: all
	@valgrind --leak-check=full --show-leak-kinds=all ./$(PROJ)