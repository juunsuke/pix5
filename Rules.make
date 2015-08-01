
#NO_OVERLOAD_NEW_DELETE = -DNO_OVERLOAD_NEW_DELETE

########################################################################################################################

HDRS += $(patsubst %,%.h,$(CLSS))
SRCS += $(patsubst %,%.cc,$(CLSS))

########################################################################################################################

ifdef OUT_LIB
OUT_LIB_BASE_DBG = $(OUT_LIB)_d
OUT_LIB_BASE_REL = $(OUT_LIB)

OUT_LIB_DBG = ../lib$(OUT_LIB_BASE_DBG).so
OUT_LIB_REL = ../lib$(OUT_LIB_BASE_REL).so

LIB_SRCS = $(SRCS)
endif

ifdef OUT_BIN
OUT_BIN_DBG = $(OUT_BIN)_d
OUT_BIN_REL = $(OUT_BIN)_r

PIXLIBS_DBG = $(patsubst %,-lpix5%_d,$(PIXLIBS))
PIXLIBS_REL = $(patsubst %,-lpix5%,$(PIXLIBS))

BIN_SRCS = $(SRCS)
endif

########################################################################################################################

DEPS = Makefile $(HDRS)

ifdef DATA
DEPS += bi_data.h
SRCS += bi_data.cc
endif

LIB_OBJS_DBG = $(patsubst %.cc,dbg_%.o,$(LIB_SRCS))
LIB_OBJS_REL = $(patsubst %.cc,rel_%.o,$(LIB_SRCS))

BIN_OBJS_DBG = $(patsubst %.cc,dbg_%.o,$(BIN_SRCS))
BIN_OBJS_REL = $(patsubst %.cc,rel_%.o,$(BIN_SRCS))

########################################################################################################################

.PHONY: debug release clean

debug: $(OUT_LIB_DBG) $(OUT_BIN_DBG)

release: $(OUT_LIB_REL) $(OUT_BIN_REL) 

clean:
	@rm -f $(OUT_LIB_DBG) $(OUT_LIB_REL) $(OUT_BIN_DBG) $(OUT_BIN_REL)
	@rm -f $(LIB_OBJS_DBG) $(LIB_OBJS_REL) $(BIN_OBJS_DBG) $(BIN_OBJS_REL)
	@rm -f bi_data.cc bi_data.h

########################################################################################################################

CC = g++
CC_OUT = -o

COPTS = -c -Wall -Werror -pthread

ifdef PKGS
COPTS += $(shell pkg-config --cflags $(PKGS))
endif

COPTS_DBG = $(COPTS) -Og -g -DDBG $(NO_OVERLOAD_NEW_DELETE)
COPTS_REL = $(COPTS) -O2 -DRELEASE

LIB_COPTS = -fPIC
LIB_COPTS_DBG = $(LIB_COPTS) $(COPTS_DBG)
LIB_COPTS_REL = $(LIB_COPTS) $(COPTS_REL)

BIN_COPTS = -I$(PIXDIR)
BIN_COPTS_DBG = $(BIN_COPTS) $(COPTS_DBG)
BIN_COPTS_REL = $(BIN_COPTS) $(COPTS_REL)


########################################################################################################################

LD = g++
LD_OUT = -o

LOPTS = -pthread

ifdef PKGS
LOPTS += $(shell pkg-config --libs $(PKGS))
endif

LOPTS_DBG = $(LOPTS) -Og -g
LOPTS_REL = $(LOPTS) -O2

LIB_LOPTS = -shared
LIB_LOPTS_DBG = $(LIB_LOPTS) $(LOPTS_DBG)
LIB_LOPTS_REL = $(LIB_LOPTS) $(LOPTS_REL)

BIN_LOPTS = -L$(PIXDIR) -Wl,-rpath=.
BIN_LOPTS_DBG = $(BIN_LOPTS) $(LOPTS_DBG) $(PIXLIBS_DBG)
BIN_LOPTS_REL = $(BIN_LOPTS) $(LOPTS_REL) $(PIXLIBS_REL)

########################################################################################################################

$(OUT_LIB_DBG): $(LIB_OBJS_DBG) $(DEPS)
	$(LD) $(LD_OUT) $(OUT_LIB_DBG) $(LIB_OBJS_DBG) $(LIB_LOPTS_DBG)

$(LIB_OBJS_DBG): dbg_%.o: %.cc $(DEPS)
	$(CC) $(CC_OUT) $@ $(LIB_COPTS_DBG) $<

########################################################################################################################

$(OUT_LIB_REL): $(LIB_OBJS_REL) $(DEPS)
	$(LD) $(LD_OUT) $(OUT_LIB_REL) $(LIB_OBJS_REL) $(LIB_LOPTS_REL)

$(LIB_OBJS_REL): rel_%.o: %.cc $(DEPS)
	$(CC) $(CC_OUT) $@ $(LIB_COPTS_REL) $<

########################################################################################################################

ifdef DATA

bi_data.cc: $(DATA) Makefile
	@echo "" > bi_data.cc
	$(foreach f,$(DATA),xxd -i $(f) >> bi_data.cc; )

bi_data.h: $(DATA) Makefile
	@echo "" > bi_data.h
	$(foreach f,$(DATA),echo "extern unsigned char $(subst -,_,$(subst /,_,$(subst .,_,$(f))))[]; extern unsigned int $(subst -,_,$(subst /,_,$(subst .,_,$(f))))_len;" >> bi_data.h; )

endif

########################################################################################################################

$(OUT_BIN_DBG): $(BIN_OBJS_DBG) $(DEPS)
	$(LD) $(LD_OUT) $(OUT_BIN_DBG) $(BIN_OBJS_DBG) $(BIN_LOPTS_DBG)

$(BIN_OBJS_DBG): dbg_%.o: %.cc $(DEPS)
	$(CC) $(CC_OUT) $@ $(BIN_COPTS_DBG) $<

########################################################################################################################

$(OUT_BIN_REL): $(BIN_OBJS_REL) $(DEPS)
	$(LD) $(LD_OUT) $(OUT_BIN_REL) $(BIN_OBJS_REL) $(BIN_LOPTS_REL)

$(BIN_OBJS_REL): rel_%.o: %.cc $(DEPS)
	$(CC) $(CC_OUT) $@ $(BIN_COPTS_REL) $<

########################################################################################################################
########################################################################################################################
########################################################################################################################


