
TARGET = pix5
DBG = 1

########################################################################################################################

CLSS  = Error EventHandler Log Cache
CLSS += Display Texture Matrix Camera Shader VertexComp VertexDef VertexBuffer VertexArray Glyph Font
CLSS += SpriteSet Sprite TexRenderer Anim
CLSS += TMLayer TMBase MapSprite
CLSS += Audio SoundClip
CLSS += GUI GuiTheme GuiElement Label Button
CLSS += Input Action
CLSS += Thread JobThread ThreadPool Job
CLSS += Lua

HDRS  = $(patsubst %,%.h,$(CLSS))
HDRS += pix5.h Str.h List.h SortedList.h FIFO.h Error.h errors.h Math.h
HDRS += VideoModeType.h VideoMode.h Color.h Rect.h BlendMode.h TextAlign.h
HDRS += TileMap.h
HDRS += Mouse.h keys.h Key.h Keyboard.h Joystick.h JoyAxis.h JoyButton.h JoyHat.h
HDRS += Mutex.h Condition.h Barrier.h RWLock.h

SRCS  = $(patsubst %,%.cc,$(CLSS))
SRCS += init.cc load_file.cc get_ms.cc
SRCS += GuiTheme_Label.cc
SRCS += lua_print.cc

DATA = sprite.vs sprite.fs LiberationSans-Regular.ttf

PKGS = sdl2 SDL2_image gl glew freetype2 vorbisfile luajit

########################################################################################################################

DEPS = Makefile $(patsubst %,%,$(HDRS)) data.h
OBJS = $(patsubst %.cc,%.o,$(SRCS))

DATAS = $(patsubst %,data/%,$(DATA))

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
	@rm -f $(LIB_DBG) $(LIB_RLS) $(OBJS) test.o test data.h data.cc $(DATA_H) $(DATA_CC)

run: test
	./test

########################################################################################################################

CC = g++ -o
LD = g++ -o

CFLAGS = -c -Wall -Werror -pthread
LFLAGS = -pthread

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

$(LIB_OUT): data.o $(OBJS) $(DEPS)
	$(LD) $(LIB_OUT) data.o $(OBJS) $(LFLAGS_LIB)

$(OBJS) data.o: %.o: %.cc $(DEPS)
	$(CC) $@ $(CFLAGS_LIB) $<

########################################################################################################################

test: $(LIB_OUT) test.o
	$(LD) test test.o $(LFLAGS_TEST)

test.o: $(LIB_OUT) test.cc $(DEPS)
	$(CC) test.o $(CFLAGS_TEST) test.cc

########################################################################################################################

data.cc: $(DATAS) Makefile
	@echo "" > data.cc
	$(foreach f,$(DATAS),xxd -i $(f) >> data.cc; )

data.h: $(patsubst %,data/%,$(DATA)) Makefile
	@echo "" > data.h
	$(foreach f,$(DATAS),echo "extern unsigned char $(subst -,_,$(subst /,_,$(subst .,_,$(f))))[]; extern unsigned int $(subst -,_,$(subst /,_,$(subst .,_,$(f))))_len;" >> data.h; )



########################################################################################################################




