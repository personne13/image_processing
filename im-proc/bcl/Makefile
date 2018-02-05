ROOT=.
LIBNAME=bcl
LIBFILENAME=lib$(LIBNAME).a

CPPFLAGS=
CFLAGS= -g -Wall -Wextra -Werror -pedantic -std=c99

HEADERS= \
	src/bcl.h \
	src/exception.h \
	src/memory.h \
	src/message.h \
	src/str.h \
	src/pnm.h

OBJ= \
	src/bcl.o \
	src/exception.o \
	src/memory.o \
	src/message.o \
	src/str.o \
	src/pnm.o

$(LIBFILENAME) : $(OBJ)
	rm -f $(LIBFILENAME)
	ar -qc $(LIBFILENAME) $(OBJ)

bcl.o: bcl.c $(HEADERS)
exception.o: exception.c exception.h memory.h
memory.o: memory.c  exception.h
message.o: message.c memory.h exception.h
str.o: str.c str.h memory.h
pnm.o: pnm.c pnm.h memory.h

$(ROOT)/lib/$(LIBFILENAME) : $(LIBFILENAME)
	cp $(HEADERS) $(ROOT)/include
	cp $(LIBFILENAME) $(ROOT)/lib

.PHONY: install checkdirs clean cleanall
install : checkdirs $(ROOT)/lib/$(LIBFILENAME) 
checkdirs :
	[ -d $(ROOT)/lib ] || mkdir $(ROOT)/lib
	[ -d $(ROOT)/include ] || mkdir $(ROOT)/include
clean:
	rm -f $(OBJ) $(LIBFILENAME)
cleanall: clean
	rm -rf $(ROOT)/lib $(ROOT)/include


