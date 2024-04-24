TARGET=blink_gpiolib

ifeq (,$(TOOLCHAIN_PREFIX))
$(error TOOLCHAIN_PREFIX is not set)
endif

ifeq (,$(CFLAGS))
$(error CFLAGS is not set)
endif

ifeq (,$(CXXFLAGS))
$(error CXXFLAGS is not set)
endif

ifeq (,$(LDFLAGS))
$(error LDFLAGS is not set)
endif

CC = $(TOOLCHAIN_PREFIX)gcc
CXX = $(TOOLCHAIN_PREFIX)g++

CFLAGS += -I$(SYSROOT)/usr/include

CXXFLAGS += -I./
CXXFLAGS += -I$(SYSROOT)/usr/include

LDFLAGS += -L$(SYSROOT)/lib
LDFLAGS += -L$(SYSROOT)/usr/lib

SOURCE = $(wildcard *.c)
CXXSOURCE = $(wildcard *.cpp)

OBJS = $(patsubst %.c,%.o,$(SOURCE))
OBJS += $(patsubst %.cpp,%.o,$(CXXSOURCE))

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c, %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@rm *.o -rf
	@rm $(OBJS) -rf
	@rm $(TARGET)

