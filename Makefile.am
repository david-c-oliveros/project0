bin_PROGRAMS = app
app_SOURCES = $(top_srcdir)/src/main.cpp \
			  $(top_srcdir)/src/stb_image.cpp \
			  $(top_srcdir)/src/Camera.cpp \
			  $(top_srcdir)/src/Shader.cpp \
			  $(top_srcdir)/src/Mesh.cpp \
			  $(top_srcdir)/src/Model.cpp
AM_CXXFLAGS = @gllibs_CFLAGS@ -Iassimp/include -Lassimp/bin -Iinclude -Iinclude/glm
AM_LDFLAGS = @gllibs_LIBS@ -lm -ldl -lassimp
