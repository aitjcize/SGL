CFLAGS = -g -Wall
LDFLAGS = -L../lib -lSGLUT -lSGL -lSGLU

all:
	$(CC) test.c -o test $(LDFLAGS) $(CFLAGS)

std:
	$(CC) test.c -o test `pkg-config --cflags --libs gl glu` -lglut

clean:
	rm -f test
