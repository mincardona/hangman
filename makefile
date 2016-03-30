# this section defines variables
# compiler exe
CC = gcc
# compiler flags
CFLAGS = -g -O2 -std=c99 -pedantic -Wall
# all object files to link (src file name with .o extension)
OBJECTS = hangman.o
# program executable and main file name (with extension)
PRGM = hangman.exe
# libraries to link with
# use -l prefex, e.g. -lmylibrary
LIBS = 

all: $(PRGM)

# compiles the main program
$(PRGM) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PRGM) $(OBJECTS) $(LIBS)
	
# % is a wildcard for compiling all files
# $< means "whatever the dependencies are"
# note that $@ means "the target"
# so, this produces object files from all source files based on their dependencies
%.o : %.c
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm *.o
	