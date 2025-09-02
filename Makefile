# Makefile
CC      := gcc
CFLAGS  := -O2 -Wall -Wextra \
           -Isrc -Isrc/core -Isrc/render -Isrc/scene -Isrc/primitives
LDLIBS  := -lGL -lGLU -lglut -lm

SRCDIR  := src
OBJDIR  := build
BINDIR  := bin
TARGET  := $(BINDIR)/ray

SRC := $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ) | $(BINDIR)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR) $(BINDIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) -r $(OBJDIR) $(BINDIR)

print-%:
	@echo '$* = $($*)'

.PHONY: all run clean print-%
