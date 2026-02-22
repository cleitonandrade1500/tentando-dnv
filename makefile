LIB_NAME := RDR2_Assalto
CXX      := clang++
CFLAGS   := -O3 -std=c++11 -fno-stack-protector -fPIC -I./source -D__PS4__
LDFLAGS  := -T linker.x -shared -nodefaultlibs

all: $(LIB_NAME).sprx

$(LIB_NAME).sprx: source/main.cpp
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	llvm-strip --strip-unneeded $@
