
TARGET = pix5
DBG = 1

########################################################################################################################

CLSS  = Error EventHandler Log Cache
CLSS += Display Texture Matrix Camera Shader VertexComp VertexDef VertexBuffer VertexArray Glyph Font
CLSS += SpriteSet Sprite TexRenderer
CLSS += Audio SoundClip
CLSS += GUI
CLSS += Input

HDRS  = $(patsubst %,%.h,$(CLSS))
HDRS += pix5.h Str.h List.h SortedList.h Error.h errors.h Math.h
HDRS += VideoModeType.h VideoMode.h Color.h Rect.h
HDRS += Mouse.h keys.h Key.h Keyboard.h 

SRCS  = $(patsubst %,%.cc,$(CLSS))
SRCS += init.cc load_file.cc

PKGS = sdl2 SDL2_image gl glew freetype2 vorbisfile

########################################################################################################################

DEPS = Makefile $(patsubst %,%,$(HDRS))
OBJS = $(patsubst %.cc,%.o,$(SRCS))

LIB_DBG = lib$(TARGET)_d.so
LIB_RLS = lib$(TARGET).so

ifdef DBG
LIB_LINK = -l$(TARGET)_d
LIB_OUT = $(LIB_DBG)
else
LIB_LINK = -l$(TARGET)
LIB_OUT = $(LIB_RLS)
endif

########################################################################################################################

.PHONY: all clean run

all: $(LIB_OUT)

clean:
	@rm -f $(LIB_DBG) $(LIB_RLS) $(OBJS) test.o test

run: test
	./test

########################################################################################################################

CC = g++ -o
LD = g++ -o

CFLAGS = -c -Wall -Werror
LFLAGS =

ifdef DBG
CFLAGS += -Og -g -DDBG
LFLAGS += -Og -g
else
CFLAGS += -O2 -DRELEASE
LFLAGS += -O2
endif

CFLAGS_LIB = $(CFLAGS) -fPIC -DPIX5 $(shell pkg-config --cflags $(PKGS))
LFLAGS_LIB = $(LFLAGS) -shared $(shell pkg-config --libs $(PKGS))

CFLAGS_TEST = $(CFLAGS)
LFLAGS_TEST = $(LFLAGS) -L. $(LIB_LINK) -Wl,-rpath=.

########################################################################################################################

$(LIB_OUT): $(OBJS) $(DEPS)
	$(LD) $(LIB_OUT) $(OBJS) $(LFLAGS_LIB)

$(OBJS): %.o: %.cc $(DEPS)
	$(CC) $@ $(CFLAGS_LIB) $<

########################################################################################################################

test: $(LIB_OUT) test.o
	$(LD) test test.o $(LFLAGS_TEST)

test.o: $(LIB_OUT) test.cc $(DEPS)
	$(CC) test.o $(CFLAGS_TEST) test.cc

########################################################################################################################




