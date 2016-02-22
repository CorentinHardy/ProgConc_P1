
INCL_DIR = .
INCL = -I$(INCL_DIR) 
DEFS = -D_DEBUG_

CFLAGS = -g -Wall -std=c99
CC = gcc 
CC_COMPILE = $(CC) $(CFLAGS) $(INCL) $(DEFS)
CC_LINK = $(CC)

SRC = src/
BIN = bin/


ALL = tst_chaleur 
ALL_EX = $(BIN)tst_chaleur.out


tst_chaleur :
	$(CC_COMPILE) -o $(BIN)Chaleur.o -c $(SRC)Chaleur.c '-lm'
	$(CC_LINK) -o $(BIN)tst_chaleur.out $(BIN)Chaleur.o '-lm'

# Built all thing and Everythings
all :
	$(MAKE) $(ALL)

# Clean up a little
clean :
	-rm -f *.o a.out *.ps \#* *~
	-rm -f *.stackdump core
	-rm -f $(BIN)*

# Clean up all 
full_clean : clean
	-rm -f $(ALL_EX) 
	-rm -rf doc/*
