CFLAGS = -g -Wall
LDFLAGS = -L../lib -lSGLUT -lSGL -lSGLU
BINS = test test_depth test_quads test_teapot

all: $(BINS)
	$(CC) test.c -o test $(LDFLAGS) $(CFLAGS)

std:
	$(CC) test.c -o test `pkg-config --cflags --libs gl glu` -lglut

clean:
	rm -f $(BINS)
