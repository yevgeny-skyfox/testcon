CC_FLAGS_OS ?= -O0 -Wall -pedantic -std=c++11


CFLAGS = $(CC_FLAGS_OS)

INCADD ?= -Icommon -Ilinux

all: TractorTest

OBJS := $(patsubst %.c, %.o, $(SRC))

SRC = common/Main.cc \
        common/SVehicle.cc \
        common/SReader.cc \
        common/SSimulator.cc \
        common/SMat.cc \
        common/SCoord.cc \
        linux/SGuard.cc \
        linux/SThread.cc \
        linux/SSysTime.cc


OBJS := $(patsubst %.cc, %.o, $(SRC))

TractorTest: $(OBJS)
	gcc  $(CFLAGS) $^ -o $@ -lm -lpthread -lstdc++

%.o: %.cc
	gcc $(CFLAGS) $(INCADD) -c $< -o $@

clean:
	rm -f $(OBJS) TractorTest

test:
	cppcheck --enable=all .
	scan-build $(MAKE)

.PHONY: all clean test
