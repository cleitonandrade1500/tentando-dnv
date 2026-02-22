# Configurações do Mod para RDR2 PS4
LIB_NAME    := Havana_Assalto_CUSA03140
TARGET_ID   := CUSA03140
GAME_VER    := 1.32

# Diretórios
OBJS_DIR    := build
BIN_DIR     := bin
SRC_DIR     := source

# SDK e Ferramentas (Puxadas do Container Docker 2026)
CC          := x86_64-main-elf-gcc
CXX         := x86_64-main-elf-g++
AS          := x86_64-main-elf-as
STRIP       := x86_64-main-elf-strip

# Flags de Compilação para v1.32
CFLAGS      := -O2 -fno-stack-protector -fno-exceptions -fPIC -D__PS4__ -DVERSION_132
CXXFLAGS    := $(CFLAGS) -std=c++17
LDFLAGS     := -Wl,-q -Wl,--eh-frame-hdr -T linker.x -shared

# Lista de arquivos para compilar (Adiciona todos .cpp da pasta source)
SOURCES     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS        := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJS_DIR)/%.o)

all: $(BIN_DIR)/$(LIB_NAME).sprx

$(BIN_DIR)/$(LIB_NAME).sprx: $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)
	$(STRIP) --strip-unneeded $@
	@echo "--------------------------"
	@echo "MOD COMPILADO COM SUCESSO!"
	@echo "--------------------------"

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR) $(BIN_DIR)
