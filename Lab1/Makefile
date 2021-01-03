#C compiler
CCOMP = gcc

#C linker
LOADER = gcc

# optymalization options
OPT = -O3

# localization of header files
INC = -I./inc

# libraries
LIB = -L./lib

# myprogram
myprogram: myprogram.c
	$(LOADER) $(INC) -c myprogram.c
	$(LOADER) $(OPT) myprogram.o tmeas.o -o myprogram $(LIB)

# tmeas	
tmeas: tmeas.c
	$(CCOMP) $(INC) -c $(OPT) tmeas.c

clean:
	rm -f *.o
	rm -f myprogram

	
