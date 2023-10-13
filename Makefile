CC ?= gcc
ZIP ?= 7za
PDFLAGS += -march=native
CFLAGS += -Wall -Wextra -Wpedantic
LDFLAGS += -lm
VDFLAGS += --leak-check=full --show-leak-kinds=all -s

SRCDIRS = $(sort $(dir $(wildcard src/*/)))
INC_PARAMS = $(addprefix -I ,$(SRCDIRS))

program ?= build/program

ZIPFILE    ?= ../zipfile.zip
CFILES      = $(wildcard src/*.c) $(wildcard src/**/*.c)

.PHONY: all clean zip run test debug

all: PDFLAGS += -O3
all: $(program)

clean:
	@rm -f $(ZIPFILE)
	@rm -rf build/

zip: clean
	$(ZIP) a $(ZIPFILE) ./*

run: $(program)
	@./$(program) $(ARGS)

debug: CFLAGS+= -g
debug: clean
debug: $(program)

$(program): $(CFILES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INC_PARAMS) -o $@ $^ $(LDFLAGS) $(PDFLAGS)

valgrind: $(program)
	valgrind $(VDFLAGS) ./$(program)
