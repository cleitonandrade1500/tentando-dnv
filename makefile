# Nome do arquivo final do mod
LIB_NAME := RDR2_Assalto_2026

# Compiladores e ferramentas (Padrão LLVM/Clang 2026)
CXX      := clang++
STRIP    := llvm-strip

# Flags de Compilação: -fno-rtti e -fno-exceptions economizam memória no PS4
CFLAGS   := -O3 -std=c++11 -fno-stack-protector -fPIC -fno-rtti -fno-exceptions -I./source -D__PS4__ -DVERSION_132
LDFLAGS  := -T linker.x -shared -nodefaultlibs -Wl,--gc-sections

# Alvo principal
all: $(LIB_NAME).sprx

$(LIB_NAME).sprx: source/main.cpp
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	$(STRIP) --strip-unneeded $@

# Regra Clean corrigida para evitar o erro da imagem
clean:
	rm -f *.sprx
	@echo "Limpeza concluída."

.PHONY: all clean
