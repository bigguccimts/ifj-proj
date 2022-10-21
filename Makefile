# Implementace překladače imperativního jazyka IFJ22 
#
# Matúš Ďurica (xduric06) 
# Ivan Mahút (xmahut01) 
# Dušan Slúka (xsluka00)
# Gabriela Paganíková (xpagan00)

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra 
RM=rm -f
PROJ=ifj22

$(PROJ): *.c clean
	$(CC) $(CFLAGS) -o $(PROJ)  *.c 

clean:
	$(RM) *.o $(PROJ)
