# Makefile
CC      := gcc
CFLAGS  := -O3 -march=native -ffast-math -fno-math-errno -Wall -Wextra -fopenmp \
           -Isrc -Isrc/core -Isrc/render -Isrc/scene -Isrc/primitives -Isrc/lighting
LDLIBS  := -lGL -lGLU -lglut -lm -fopenmp

SRCDIR  := src
OBJDIR  := build
BINDIR  := bin
TARGET  := $(BINDIR)/ray

SRC := $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))


all: $(TARGET)

$(TARGET): $(OBJ) | $(BINDIR)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

# regra genérica de compilação com criação de subpastas em build/
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR) $(BINDIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

# para laptops híbridos (NVIDIA PRIME)
run-nvidia: $(TARGET)
	prime-run ./$(TARGET)

clean:
	$(RM) -r $(OBJDIR) $(BINDIR)

print-%:
	@echo '$* = $($*)'

.PHONY: all run run-nvidia clean print-%
