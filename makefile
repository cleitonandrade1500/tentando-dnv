LIB_NAME := RDR2_Assalto_PRO_2026
CXX      := clang++
CFLAGS   := -O2 -std=c++11 -fno-stack-protector -fPIC -fno-rtti -fno-exceptions -I./source
# LDFLAGS corrigido para exportar símbolos globais
LDFLAGS  := -shared -Wl,-gc-sections -Wl,-export-dynamic -nodefaultlibs -Wl,--unresolved-symbols=ignore-all

all: $(LIB_NAME).sprx

$(LIB_NAME).sprx: source/main.cpp
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	llvm-strip --strip-unneeded $@

clean:
	rm -f *.sprx
