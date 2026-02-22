LIB_NAME := MeuModRDR2_CUSA03140
CC       := x86_64-main-elf-gcc
CXX      := x86_64-main-elf-g++
STRIP    := x86_64-main-elf-strip

CFLAGS   := -O3 -fno-stack-protector -fPIC -D__PS4__ -DVERSION_132
LDFLAGS  := -Wl,-q -shared -T linker.x

all: $(LIB_NAME).sprx

$(LIB_NAME).sprx: source/main.cpp
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	$(STRIP) --strip-unneeded $@

clean:
	rm -f *.sprx
