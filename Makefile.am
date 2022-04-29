bin_PROGRAMS = app
app_SOURCES = $(top_srcdir)/src/main.cpp \
			  $(top_srcdir)/src/stb_image.cpp \
			  $(top_srcdir)/src/Game.cpp \
			  $(top_srcdir)/src/Camera.cpp \
			  $(top_srcdir)/src/Shader.cpp \
			  $(top_srcdir)/src/Mesh.cpp \
			  $(top_srcdir)/src/Model.cpp \
			  $(top_srcdir)/src/Object.cpp \
			  $(top_srcdir)/src/PointLight.cpp \
			  $(top_srcdir)/src/SpotLight.cpp \
			  $(top_srcdir)/src/TickTimer.cpp \
			  $(top_srcdir)/src/BoxCollider.cpp \
			  $(top_srcdir)/src/Text.cpp
AM_CXXFLAGS = @gllibs_CFLAGS@ -Wfatal-errors -Iassimp/include -Lassimp/bin -Iinclude -Iinclude/glm \
			  -I/usr/include/freetype2 -I/usr/include/freetype2/freetype
AM_LDFLAGS = @gllibs_LIBS@ -lm -ldl -lassimp -lfreetype
