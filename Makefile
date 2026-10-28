CC = gcc
CFLAGS = -Wall -pedantic -Werror
LFLAGS = -lm
RM = /bin/rm
OBJ = prime.o primeFunction.o
EXEC = prime

ifdef CUBE
CFLAGS += -D CUBE
CUBE : clean $(EXEC)
endif

$(EXEC) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

clean : 
	$(RM) $(EXEC) $(OBJ)

SRCS = prime.c primeFunction.c

depend : 
	$(RM) .depend
	makedepend -f- -- $(CFLAGS) -- $(SRCS) > .depend 2>/dev/null

-include .depend
