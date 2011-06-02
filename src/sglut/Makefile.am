AM_CFLAGS = -Wall -O2
lib_LTLIBRARIES = libSGLUT.la
libSGLUT_la_LDFLAGS = \
		-lX11 \
		-version-info $(LIBSGL_CURRENT):$(LIBSGL_REVISION):$(LIBSGL_AGE)
libSGLUT_la_SOURCES = \
		sglut_api.h \
		types.h \
		context.c context.h
