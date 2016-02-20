
INCL_DIR = .
INCL = -I$(INCL_DIR) 
DEFS = -D_DEBUG_

CFLAGS = -g -Wall -std=c99
CC = gcc 
CC_COMPILE = $(CC) $(CFLAGS) $(INCL) $(DEFS)
CC_LINK = $(CC)

%.o :	%.c
	$(CC_COMPILE) -c $*.c

% :	%.c
	$(CC_COMPILE) -o $@ $*.c


ALL = tst_chaleur 
ALL_EX = tst_chaleur.out


tst_chaleur : Chaleur.o
	$(CC_LINK) -o tst_chaleur.out Chaleur.o

# Built all thing and Everythings
all :
	$(MAKE) $(ALL)

# Clean up a little
clean :
	-rm -f *.o a.out *.ps \#* *~
	-rm -f *.stackdump core

# Clean up all 
full_clean : clean
	-rm -f $(ALL_EX) 
	-rm -rf doc/*
