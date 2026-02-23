LIB_NAME := RDR2_Assalto_PRO_2026
CXX      := clang++
STRIP    := llvm-strip

# CFLAGS: -nostdinc evita que o compilador procure arquivos que o Ubuntu não tem
CFLAGS   := -target x86_64-pc-freebsd12-elf -O2 -std=c++11 \
            -fno-stack-protector -fPIC -fno-rtti -fno-exceptions \
            -fms-extensions -fno-builtin -I./source -D__PS4__ -nostdinc -nostdinc++

# LDFLAGS: -nostdlib resolve os erros 'cannot find crti.o' do seu print
LDFLAGS  := -target x86_64-pc-freebsd12-elf -shared -nostdlib \
            -Wl,-gc-sections -Wl,-export-dynamic \
            -Wl,-z,max-page-size=0x4000 -Wl,-z,notext \
            -Wl,--unresolved-symbols=ignore-all

all: $(LIB_NAME).sprx

$(LIB_NAME).sprx: source/main.cpp source/natives.h
	$(CXX) $(CFLAGS) source/main.cpp -o $@ $(LDFLAGS)
	$(STRIP) --strip-unneeded $@

clean:
	rm -f *.sprx

.PHONY: all clean
