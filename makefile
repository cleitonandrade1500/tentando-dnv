# Nome do arquivo final gerado
LIB_NAME := RDR2_Assalto_PRO_2026

CXX      := clang++
STRIP    := llvm-strip

# CFLAGS: Removido stdinc++ para evitar o erro 'file not found'
CFLAGS   := -target x86_64-pc-freebsd12-elf -O2 -std=c++11 \
            -fno-stack-protector -fPIC -fno-rtti -fno-exceptions \
            -fms-extensions -I./source -D__PS4__ -nostdinc++

# LDFLAGS: Configurações para o GoldHEN 2.4b
LDFLAGS  := -target x86_64-pc-freebsd12-elf -shared \
            -Wl,-gc-sections -Wl,-export-dynamic -nodefaultlibs \
            -Wl,-z,max-page-size=0x4000 -Wl,-z,notext

all: $(LIB_NAME).sprx

$(LIB_NAME).sprx: source/main.cpp source/natives.h
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	$(STRIP) --strip-unneeded $@

clean:
	rm -f *.sprx

.PHONY: all clean
