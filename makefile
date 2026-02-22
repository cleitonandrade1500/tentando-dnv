LIB_NAME := RDR2_Assalto_PRO_2026
CXX      := clang++
CFLAGS   := -O3 -std=c++11 -fno-stack-protector -fPIC -fno-rtti -fno-exceptions -I./source -D__PS4__
LDFLAGS  := -T linker.x -shared -nodefaultlibs -Wl,--gc-sections
all: $(LIB_NAME).sprx
$(LIB_NAME).sprx: source/main.cpp
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	llvm-strip --strip-unneeded $@
clean:
	rm -f *.sprx
.PHONY: all clean
