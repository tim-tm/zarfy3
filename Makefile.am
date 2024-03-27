AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = zarfy3
zarfy3_SOURCES = \
		src/main.c \
		src/screens.c \
		src/screens.h
zarfy3_CPPFLAGS = @GTK3_CFLAGS@
zarfy3_LDFLAGS = @GTK3_LIBS@ -lX11 -lXrandr
