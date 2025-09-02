# Makefile
CC      := gcc
CFLAGS  := -O2 -Wall -Wextra -Isrc
LDLIBS  := -lGL -lGLU -lglut -lm

SRCDIR  := src
INCDIR  := src
OBJDIR  := build
BINDIR  := bin
TARGET  := $(BINDIR)/ray

SRC     := $(SRCDIR)/main.c $(SRCDIR)/render.c $(SRCDIR)/scene.c
OBJ     := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

# ===== targets =====
all: $(TARGET)

$(TARGET): $(OBJ) | $(BINDIR)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
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
