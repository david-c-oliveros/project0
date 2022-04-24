AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = app
app_SOURCES = $(top_srcdir)/src/main.cpp \
			  $(top_srcdir)/src/stb_image.cpp \
			  $(top_srcdir)/src/Camera.cpp \
			  $(top_srcdir)/src/Shader.cpp
AM_CXXFLAGS = @gllibs_CFLAGS@ -DSUBDIRS -Iinclude -Iinclude/glm -Iassimp/include -Lassimp/bin
AM_LDFLAGS = @gllibs_LIBS@ -lm -ldl -lassimp
