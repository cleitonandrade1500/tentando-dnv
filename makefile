# Nome do arquivo de saída
LIB_NAME := RDR2_Assalto_PRO_2026

# Ferramentas de compilação (Ambiente Ubuntu GitHub Actions)
CXX      := clang++
STRIP    := llvm-strip

# CFLAGS: Ajustado para o Kernel do PS4 e versão 1.32
# -target x86_64-pc-freebsd12-elf: Alvo correto para o OS do PS4
# -fPIC: Obrigatório para bibliotecas dinâmicas .sprx
# -fms-extensions: Permite extensões necessárias para plugins de sistema
CFLAGS   := -target x86_64-pc-freebsd12-elf \
            -O2 -std=c++11 -fno-stack-protector -fPIC -fno-rtti -fno-exceptions \
            -fms-extensions -I./source -D__PS4__

# LDFLAGS: Configurações cruciais para o Plugin Loader do GoldHEN
# -shared: Gera o formato .sprx (Dynamic Library)
# -Wl,-z,notext: Permite realocação de memória (evita crash no Havana)
# -Wl,-z,max-page-size=0x4000: Alinhamento de memória padrão do PS4
LDFLAGS  := -target x86_64-pc-freebsd12-elf -shared \
            -Wl,-gc-sections -Wl,-export-dynamic -nodefaultlibs \
            -Wl,-z,max-page-size=0x4000 -Wl,-z,notext

# Arquivos fonte
SRC      := source/main.cpp
HDR      := source/natives.h

# Regra principal
all: $(LIB_NAME).sprx

# Processo de compilação e limpeza de símbolos inúteis (Strip)
$(LIB_NAME).sprx: $(SRC) $(HDR)
	$(CXX) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)
	$(STRIP) --strip-unneeded $@

# Limpeza para nova compilação
clean:
	rm -f *.sprx

.PHONY: all clean
