# Nome do arquivo final
LIB_NAME := RDR2_Assalto_PRO_2026

# Compilador e Stripper
CXX      := clang++
STRIP    := llvm-strip

# CFLAGS: -std=c++17 é ESSENCIAL para corrigir o erro de 'void'
# -target x86_64-pc-freebsd12-elf garante que o PS4 reconheça o mod
CFLAGS   := -target x86_64-pc-freebsd12-elf \
            -O2 -std=c++17 -fno-stack-protector -fPIC -fno-rtti -fno-exceptions \
            -fms-extensions -I./source -D__PS4__

# LDFLAGS: Configurações de memória para o GoldHEN 2.4b
LDFLAGS  := -target x86_64-pc-freebsd12-elf -shared \
            -Wl,-gc-sections -Wl,-export-dynamic -nodefaultlibs \
            -Wl,-z,max-page-size=0x4000 -Wl,-z,notext

# Regra principal
all: $(LIB_NAME).sprx

# Compilação
$(LIB_NAME).sprx: source/main.cpp source/natives.h
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	$(STRIP) --strip-unneeded $@

# Limpeza
clean:
	rm -f *.sprx

.PHONY: all clean
