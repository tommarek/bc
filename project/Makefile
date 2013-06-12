INCLUDE=-I/usr/include/ -I/usr/include/SDL -I/usr/X11R6/include/GL/ -I.

NASM=nasm
CP=cp

#debug flag
D=1

ifneq ($(D),1)

DEBUG=-DNDEBUG
CFLAGS+=-fno-exceptions # disable exceptions
CFLAGS+=-nostdlib -nodefaultlibs -nostartfiles -emain # remove std module
CFLAGS+=-ffast-math # fast but incorrect math, does not set ERRNO
CFLAGS+=-funsafe-loop-optimizations # unsafe loops ;-)
CFLAGS+=-fsingle-precision-constant # do not convert float constant to double
CFLAGS+=-fno-stack-check
CFLAGS+=-fno-keep-inline-functions # inline functions does not stay in program
CFLAGS+=-O3
CFLAGS+=-Os
CFLAGS+=-s

else

DEBUG=-g
CFLAGS+=-O0 -fno-inline

endif
CFLAGS+=-DX11 #-DNOSHADOW
CFLAGS+=$(DEBUG)
CFLAGS+=-Wall -std=gnu99 `sdl-config --cflags` $(INCLUDE)

NASMFLAGS+=-felf
NASMFLAGS+=$(DEBUG) 

LIBS = -lgcc -lGL -lGLU -lSDL

LDFLAGS = $(LIBDIR) $(LIBS)
LDFLAGS += -Wl,--gc-sections# -Wl,--print-gc-sections -Wl,--print-map `sdl-config --libs`

CFILES=$(shell find framework/ -type f -name '*.c')
CFILES+=$(shell find demo/ -type f -name '*.c' -a ! -name '*_generated.c' -a ! -name '*_instrument.c')

MODELS=$(wildcard demo/meshes/models/*.raw)
INSTRUMENTS=$(wildcard demo/music/instruments/*.wav)

OFILES=$(CFILES:.c=.o) $(MODELS:.raw=_generated.o) $(INSTRUMENTS:.wav=_instrument.o)


PROG=everythingIsPossible

ifneq ($(D),1)
all: packed
else
all: $(PROG)
endif

packed: $(PROG).packed

.PRECIOUS: %_generated.h %_generated.c %_instrument.c %instrument.h
.PHONY: all packed clean mrproper

%.packed: %
	$(CP) $^ $@
	upx --ultra-brute $@

%_generated.h %_generated.c:%.raw
	./framework/scripts/raw2c.py -a 0.001 -b 16 -i 5000 -f $^
	
%_instrument.h %_instrument.c:%.wav
	./framework/scripts/instrumentsFFT.py -i $^

$(PROG): $(OFILES)
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $^

clean:
	$(RM) $(OFILES) $(PROG) $(PROG).packed $(OFILES:.o=.d)

mrproper: clean
	$(RM) $(MODELS:.raw=_generated.c) $(MODELS:.raw=_generated.h) $(INSTRUMENTS:.wav=_instrument.h)

%.d:%.c
	$(CC) -MM -MG $(CPPFLAGS) $< | sed '1 s|:| $@:|' > $@


-include $(OFILES:.o=.d)
