CFLAGS = -g -Wall -O3
LDFLAGS = -L../lib -lSGLUT -lSGL -lSGLU
#LDFLAGS = $(shell pkg-config --libs --cflags gl glu) -lglut
BINS = test test_depth test_quads test_teapot

all: $(BINS)
	$(CC) test.c -o test $(LDFLAGS) $(CFLAGS)

std:
	$(CC) test.c -o test `pkg-config --cflags --libs gl glu` -lglut

clean:
	rm -f $(BINS)
