
OBJ = file_reader.o list.o parser.o dictionary.o main.o


main: $(OBJ)
	gcc -fopenmp -o main $(OBJ)

%.o: %.c
	gcc -g -fopenmp -c $<
	




