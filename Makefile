CC ?= gcc
ZIP ?= 7za
PDFLAGS += -march=native
CFLAGS += -Wall -Wextra -Wpedantic
LDFLAGS += -lm
VDFLAGS += --leak-check=full --show-leak-kinds=all -s

SRCDIRS = $(sort $(dir $(wildcard src/*/)))
INC_PARAMS = $(addprefix -I ,$(SRCDIRS))

PEER1 ?= build/peer1
PEER2 ?= build/peer2

ZIPFILE    ?= ../zipfile.zip
CFILES      = $(wildcard src/*.c) $(wildcard src/**/*.c)

.PHONY: all clean zip run test debug

all: PDFLAGS += -O3
all: $(PEER1) $(PEER2)

clean:
	@rm -f $(ZIPFILE)
	@rm -rf build/

zip: clean
	$(ZIP) a $(ZIPFILE) ./*

run1: $(PEER1)
	@./$(PEER1) $(ARGS)

run2: $(PEER2)
	@./$(PEER2) $(ARGS)

debug: CFLAGS+= -g
debug: clean
debug: $(PEER1)

$(PEER1): $(CFILES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INC_PARAMS) -o $@ $^ $(LDFLAGS) $(PDFLAGS)

$(PEER2): $(CFILES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INC_PARAMS) -o $@ $^ $(LDFLAGS) $(PDFLAGS)

valgrind: $(PEER1)
	valgrind $(VDFLAGS) ./$(PEER1)
