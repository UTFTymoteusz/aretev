TARGET ?= $(shell cat cfg/target)
HOST   := $(shell echo ${TARGET} | sed 's/-/\//')
ARCH   := $(shell echo ${HOST} | grep -Po '^[^/]+')

include cfg/${ARCH}/none.cfg
include cfg/${HOST}.cfg

MKDIR = mkdir -p

BIN      := ./build/artkrnl/
BOOT     := boot/
DEP_DEST := $(BIN)dep/
OBJ_DEST := $(BIN)obj/

CXXFILES  := $(shell find . -type f -name '*.cpp' -not -path './arch/*' -not -path './mod/*' -not -path './lib/*')  \
	$(shell find './arch/stub/.' -type f -name '*.cpp') \
	$(shell find './arch/$(HOST)/.' -type f -name '*.cpp' -not -path './arch/$(HOST)/./mod/*') \
	$(shell find './arch/$(ARCH)/none/.' -type f -name '*.cpp' -not -path './arch/$(ARCH)/none/./mod/*')
HXXFILES  := $(shell find . -type f -name '*.hpp' -not -path './arch/*' -not -path './mod/*' -not -path './lib/*')  \
	$(shell find './arch/stub/.' -type f -name '*.hpp') \
	$(shell find './arch/$(HOST)/.' -type f -name '*.hpp' -not -path './arch/$(HOST)/./mod/*') \
	$(shell find './arch/$(ARCH)/none/.' -type f -name '*.hpp' -not -path './arch/$(ARCH)/none/./mod/*')
ASMFILES  := $(shell find . -type f -name '*.asm' -not -path './arch/*' -not -path './mod/*')  \
	$(shell find './arch/$(HOST)/.' -type f -name '*.asm' -not -path './arch/$(HOST)/./mod/*') \
	$(shell find './arch/$(ARCH)/none/.' -type f -name '*.asm' -not -path './arch/$(ARCH)/none/./mod/*')
PSFFILES  := $(shell find . -type f -name '*.psf' -not -path './arch/*' -not -path './mod/*')  \
	$(shell find './arch/$(HOST)/.' -type f -name '*.psf' -not -path './arch/$(HOST)/./mod/*') \
	$(shell find './arch/$(ARCH)/none/.' -type f -name '*.psf' -not -path './arch/$(ARCH)/none/./mod/*')
ASMRFILES := $(shell find . -type f -name '*.asmr' -not -path './arch/*' -not -path './mod/*')  \
	$(shell find './arch/$(HOST)/.' -type f -name '*.asmr' -not -path './arch/$(HOST)/./mod/*') \
	$(shell find './arch/$(ARCH)/none/.' -type f -name '*.asmr' -not -path './arch/$(ARCH)/none/./mod/*')

LIBS := ./build/libafxheap/libafxheap.a \
		./build/libafxelf/libafxelf.a

OBJS    := $(patsubst %.o, $(OBJ_DEST)%.o, $(CXXFILES:.cpp=.cpp.o) $(ASMFILES:.asm=.asm.o) $(PSFFILES:.psf=.psf.o) $(ASMRFILES:.asmr=.asmr.o))
VERSION := $(shell date -u '+%d.%m.%Y').$(shell printf "%05d" $(shell date -d "1970-01-01 UTC $$(date -u +%T)" +%s))

ISO := $(BIN)grubiso/
SYS := $(ISO)sys/

GFLAGS  := $(GFLAGS) -O0 -Wall -Wextra -nostdlib -pipe -lgcc -ffreestanding -g
ASFLAGS := $(ASFLAGS) -g

INCLUDES := -I. -Iinclude/	     \
	-Iarch/$(HOST)/			     \
	-Iarch/$(ARCH)/none/	     \
	-Iarch/$(HOST)/include/	     \
	-Iarch/$(ARCH)/none/include/ \
	-Ilib/libafxheap/include/    \
	-Ilib/libafxelf/include/

KERNEL_SRC := $(shell pwd)/

CXXFLAGS := $(GFLAGS) $(CXXFLAGS) \
	-std=c++20				      \
	-fanalyzer					  \
	-fno-rtti				      \
	-fno-exceptions			      \
	-fno-pic			   	      \
	-fno-stack-protector          \
	-fno-omit-frame-pointer       \
	-fvisibility=hidden		      \
	-DARCH="\"$(ARCH)\""          \
	-DVERSION="\"$(VERSION)\""    \
	-DDEBUG="$(DEBUG)"            \
	$(INCLUDES)

LDFLAGS := $(GFLAGS) $(LDFLAGS)	\
	-no-pie
	
all: $(OBJS)	
	@$(MKDIR) $(ISO) $(SYS)

	cd lib/ && $(MAKE) all CC="$(CC)" CXX="$(CXX)" LD="$(LD)"

#	@cd mod/core && $(MAKE) all ROOT_DIR="$(ROOT_DIR)" KERNEL_SRC="$(KERNEL_SRC)" && cd ../..
#	@cd mod/init && $(MAKE) all ROOT_DIR="$(ROOT_DIR)" KERNEL_SRC="$(KERNEL_SRC)" && cd ../..
#	@cd arch/$(ARCH)/mod/core && $(MAKE) all ROOT_DIR="$(ROOT_DIR)" KERNEL_SRC="$(KERNEL_SRC)" && cd ../../../..
#	@cd arch/$(ARCH)/mod/init && $(MAKE) all ROOT_DIR="$(ROOT_DIR)" KERNEL_SRC="$(KERNEL_SRC)" && cd ../../../..

	@$(CXX) $(OBJS) $(LDFLAGS) -T arch/$(HOST)/link.ld $(LIBS) -o $(SYS)artkrnl
#	@cp $(SYS)artkrnl artsyms
#	@strip $(SYS)artkrnl

	@printf '\033[0;36m%s\033[0m: Done building for \033[0;36m%s\033[0m\033[0K\n' artkrnl $(shell echo ${HOST} | sed 's/\//-/')

copy:
	@cp -u $(SYS)artkrnl    "$(ROOT_DIR)sys/"
	@cp -u $(SYS)artkrnl.sf "$(ROOT_DIR)sys/sym/"

include $(shell find $(DEP_DEST) -type f -name *.d)

iso: all
	grub-mkrescue -o /tmp/art.iso $(ISO)

clean:
	cd lib/ && $(MAKE) clean

#	cd mod/core && $(MAKE) clean && cd ../..
#	cd mod/init && $(MAKE) clean && cd ../..

#	cd arch/$(ARCH)/mod/core && $(MAKE) clean && cd ../../../..
#	cd arch/$(ARCH)/mod/init && $(MAKE) clean && cd ../../../..

	rm -rf $(DEP_DEST)
	rm -rf $(OBJ_DEST)

$(OBJ_DEST)%.cpp.o : %.cpp
	@$(MKDIR) ${@D}
	@$(MKDIR) $(dir $(DEP_DEST)$*)

	@printf '\033[0;36m%s\033[0m: Building \033[0;36m$(<)\033[0m\033[0K\n' artkrnl
	@$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MT $@ -MF $(DEP_DEST)$*.cpp.d

$(OBJ_DEST)%.asm.o : %.asm
	@$(MKDIR) ${@D}

	@printf '\033[0;36m%s\033[0m: Building \033[0;36m$(<)\033[0m\033[0K\n' artkrnl
	@$(AS) $(ASFLAGS) $< -o $@

$(OBJ_DEST)%.psf.o : %.psf
	@$(MKDIR) ${@D}

	@printf '\033[0;36m%s\033[0m: Building \033[0;36m$(<)\033[0m\033[0K\n' artkrnl
	@objcopy -B i386:x86-64 -O elf64-x86-64 -I binary $< $@

$(OBJ_DEST)%.asmr.o : %.asmr
	@$(MKDIR) ${@D}
	@$(AS) -f bin -o $@ $<
	
	@printf '\033[0;36m%s\033[0m: Building \033[0;36m$(<)\033[0m\033[0K\n' artkrnl
	@objcopy -B i386:x86-64 -O elf64-x86-64 -I binary $@ $@