#modifier la valeur de ce paramètre pour compiler avec options debug ou optimisation
TYPE=opt
#	debug / opt
	
CC= g++ -Iinclude/
CFLAGS = -W -Wall -ansi -pedantic -pipe
OPT= -O3
EXEC= prog
ifeq ($(TYPE),debug)
	FLAGS=$(CFLAGS) -g
else 
	ifeq ($(TYPE),opt)
		FLAGS=$(CFLAGS) $(OPT)
	else
		FLAGS=$(CFLAGS)
	endif
endif

all : $(EXEC)
prog: main.o
	$(CC) $^ -o bin/$@ $(FLAGS)

%.o : src/%.cpp
	$(CC)  -c $^ $(FLAGS)
clean:
	@ rm -f *.o
mrproper: clean
	@ rm -f bin/$(EXEC) 
