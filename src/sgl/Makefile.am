SRC_ROOT = ..
AM_CFLAGS = -I$(SRC_ROOT) -Wall -O2 -g
lib_LTLIBRARIES = libSGL.la
libSGL_la_LDFLAGS = \
		-lm \
		-version-info $(LIBSGL_CURRENT):$(LIBSGL_REVISION):$(LIBSGL_AGE)
libSGL_la_SOURCES = \
		config.h \
		macro.h \
		sglapi.h \
		sglheader.h \
		types.h \
		context.c context.h \
		geometry.c geometry. h \
		polygon.c polygon. h \
		logging.c logging.h \
		matrix.c matrix .h \
		viewport.c viewport.h \
		depth.c depth.h \
		framebuffer.c framebuffer.h \
		renderbuffer.c renderbuffer.h \
		pipeline.c pipeline.h \
		rasterize.c rasterize.h \
		$(SRC_ROOT)/math/m_vector.c $(SRC_ROOT)/math/m_vector.h \
		$(SRC_ROOT)/math/m_matrix.c $(SRC_ROOT)/math/m_matrix.h
