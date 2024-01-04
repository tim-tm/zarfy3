CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
DEFINES=
INCLUDES=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0` -lX11 -lXrandr

SRCDIR=src
BUILDDIR=build

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

BINARY=$(BUILDDIR)/zarfy3

.PHONY: all setup clean destroy

all: setup $(BINARY)

$(BINARY): $(BUILDDIR)/$(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $(BINARY) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

setup:
	mkdir -p $(BUILDDIR)

clean:
	rm -f $(BINARY)
	rm -f $(OBJ)

destroy:
	rm -rf $(BUILDDIR)

install: all
	cp $(BINARY) /usr/bin

run: all
	./build/zarfy3
