CC	 = gcc
FLAGS    = -O2 -Wall
LDFLAGS  = -lm
SRCS     = qr.c
OBJS     = $(SRCS:.c=.o)
EXEC	 = qr

all: qr


.c.o:
	$(CC) $(FLAGS) -c $< -o $@
	
	
$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(FLAGS) qr.o -o qr


clean:
	rm -f *.o *~ $(EXEC)


# Dependencies
