STATIC_LINKING := 0
AR             := ar

ifeq ($(platform),)
platform = unix
endif

# system platform
system_platform = unix
TARGET_NAME := spring
LIBM		= -lm

ifeq ($(ARCHFLAGS),)
   ARCHFLAGS = -arch i386 -arch x86_64
endif

ifeq ($(STATIC_LINKING), 1)
EXT := a
endif

EXT ?= so
TARGET := $(TARGET_NAME)_libretro.$(EXT)
fpic := -fPIC
SHARED := -shared -Wl,--version-script=link.T -Wl,--no-undefined

LDFLAGS += $(LIBM)

ifeq ($(DEBUG), 1)
   CFLAGS += -O0 -g
else
   CFLAGS += -O3
endif

OBJECTS := spring-libretro.o
CFLAGS += -I../../libretro-common/include -Wall -pedantic $(fpic)

all: $(TARGET)

$(TARGET): $(OBJECTS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJECTS)
else
	$(CC) $(fpic) $(SHARED) $(INCLUDES) -o $@ $(OBJECTS) $(LDFLAGS)
endif

%.o: %.c
	$(CC) $(CFLAGS) $(fpic) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean

