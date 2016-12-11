
OBJ = file_reader.o list.o parser.o dictionary.o main1.o


main: $(OBJ)
	gcc -fopenmp -o main $(OBJ)

%.o: %.c
	gcc -fopenmp -c $<
	




