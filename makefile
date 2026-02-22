LIB_NAME := RDR2_Assalto_PRO_2026
CXX      := clang++
CFLAGS   := -O2 -std=c++11 -fno-stack-protector -fPIC -fno-rtti -fno-exceptions -I./source -D__PS4__
# Flags críticas: -z notext permite rodar em FPKGs sem erros de realocação
LDFLAGS  := -shared -Wl,-gc-sections -Wl,-export-dynamic -nodefaultlibs -Wl,-z,max-page-size=0x4000 -Wl,-z,notext -Wl,--unresolved-symbols=ignore-all

all: $(LIB_NAME).sprx

$(LIB_NAME).sprx: source/main.cpp
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	llvm-strip --strip-unneeded $@

clean:
	rm -f *.sprx
