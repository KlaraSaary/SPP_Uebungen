
OBJ = file_reader.o list.o parser.o dictionary.o main.o



all:

main: $(OBJ) 
	gcc -o main $(OBJ) -lgomp -fopenmp 

%.o: %.c
	gcc -c $<
	
