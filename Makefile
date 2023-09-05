CC ?= gcc
ZIP ?= 7za
PDFLAGS += -march=native
CFLAGS += -Wall -Wextra -Wpedantic
LDFLAGS += -lm
VDFLAGS += --leak-check=full --show-leak-kinds=all -s

SRCDIRS = $(sort $(dir $(wildcard src/*/)))
INC_PARAMS = $(addprefix -I ,$(SRCDIRS))

EXECUTABLE ?= build/program

ZIPFILE    ?= ../zipfile.zip
CFILES      = $(wildcard src/*.c) $(wildcard src/**/*.c)

.PHONY: all clean zip run test debug

all: PDFLAGS += -O3
all: $(EXECUTABLE)

clean:
	@rm -f $(ZIPFILE)
	@rm -rf build/
	@rm -rf out/*.csv

zip: clean
	$(ZIP) a $(ZIPFILE) ./*

run: $(EXECUTABLE)
	@./$(EXECUTABLE) $(ARGS)

debug: CFLAGS+= -g
debug: clean
debug: $(EXECUTABLE)

$(EXECUTABLE): $(CFILES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INC_PARAMS) -o $@ $^ $(LDFLAGS) $(PDFLAGS)

valgrind: $(EXECUTABLE)
	valgrind $(VDFLAGS) ./$(EXECUTABLE)
