# Nome do arquivo final .sprx
LIB_NAME := RDR2_Assalto_PRO_2026

# Compilador e Stripper (Padrão do GitHub Actions / Ubuntu)
CXX      := clang++
STRIP    := llvm-strip

# CFLAGS: -I./source garante que o compilador encontre o seu natives.h
# -fPIC e -fno-exceptions são obrigatórios para plugins de PS4
CFLAGS   := -O2 -std=c++11 -fno-stack-protector -fPIC -fno-rtti -fno-exceptions -I./source -D__PS4__

# LDFLAGS: O segredo para o GoldHEN carregar o mod
# -shared: Cria a biblioteca dinâmica .sprx
# -Wl,-export-dynamic: Torna o 'module_start' visível para o GoldHEN
# -Wl,-z,notext: Essencial para rodar em FPKGs (permite realocação)
# -Wl,-z,max-page-size=0x4000: Alinhamento de memória do Kernel do PS4
LDFLAGS  := -shared -Wl,-gc-sections -Wl,-export-dynamic -nodefaultlibs \
            -Wl,-z,max-page-size=0x4000 -Wl,-z,notext \
            -Wl,--unresolved-symbols=ignore-all

# Regra principal
all: $(LIB_NAME).sprx

# Compilação do main.cpp usando o natives.h da pasta source
$(LIB_NAME).sprx: source/main.cpp source/natives.h
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	$(STRIP) --strip-unneeded $@

# Limpeza
clean:
	rm -f *.sprx

.PHONY: all clean
