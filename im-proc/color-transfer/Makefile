ROOT=../bcl

CPPFLAGS = -I$(ROOT)/include
CFLAGS   = -Wall -Wextra -pedantic -std=c99
LDFLAGS  = -L$(ROOT)/lib
LDLIBS   = -lbcl -lm

BIN=\
	color-transfer\

.PHONY: all
all:$(BIN)

.PHONY: clean cleanall
clean:
	$(RM) *.o *~ *.ppm
cleanall: clean
	$(RM) $(BIN)
