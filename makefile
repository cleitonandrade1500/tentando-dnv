LIB_NAME := MeuModAssalto
CC       := clang
CXX      := clang++

CFLAGS   := -O3 -fno-stack-protector -fPIC -I./source -D__PS4__ -DVERSION_132
LDFLAGS  := -Wl,-q -shared -T linker.x

all: $(LIB_NAME).sprx

$(LIB_NAME).sprx: source/main.cpp
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	llvm-strip --strip-unneeded $@

clean:
	rm -f *.sprx
