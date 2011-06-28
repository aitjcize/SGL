AM_CFLAGS = -Wall -O2 -g

AM_LDFLAGS = \
	$(top_builddir)/src/sglut/libSGLUT.la \
	$(top_builddir)/src/sglu/libSGLU.la \
	$(top_builddir)/src/sgl/libSGL.la

bin_PROGRAMS =  test test_depth test_quads test_color test_teapot

test_SOURCES = test.c
test_depth_SOURCES = test_depth.c
test_quads_SOURCES = test_quads.c
test_color_SOURCES = test_color.c
test_teapot_SOURCES = test_teapot.c
