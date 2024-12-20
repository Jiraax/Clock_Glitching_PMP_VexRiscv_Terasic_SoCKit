# BUILD_DIR?= ../../../../../build/ArtyA7_100T_ChipWhisperer_2022_08
# BUILD_DIR?= ../../../../../build/ArtyA7100TCW
# BUILD_DIR?=../../build/ArtyA7_100T_ChipWhisperer_2022_08_platform/

BUILD_DIR?=$${HOME}/litex/build/CSXFC6D6F31C6N

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

OBJECTS_Vexriscv   = start_Vexriscv.o trap_handler.o simpleserial.o pmp.o main.o

# To change by ROM or RAM
LINKER = linker_rom.ld

all: glitch-pmp.bin

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
ifneq ($(OS),Windows_NT)
	chmod -x $@
endif

vpath %.a $(PACKAGES:%=../%)

glitch-pmp.elf: $(OBJECTS_Vexriscv)
	$(CC) $(LDFLAGS) -T $(LINKER) -N -o $@ \
		$(OBJECTS_Vexriscv) \
		$(PACKAGES:%=-L$(BUILD_DIR)/software/%) \
		-Wl,--whole-archive \
		-Wl,--gc-sections \
		-Wl,-Map,$@.map \
		$(LIBS:lib%=-l%)

ifneq ($(OS),Windows_NT)
	chmod -x $@
endif

# pull in dependency info for *existing* .o files
-include $(OBJECTS_Vexriscv:.o=.d)


#donut.o: CFLAGS   += -w

VPATH = $(BIOS_DIRECTORY):$(BIOS_DIRECTORY)/cmds:$(CPU_DIRECTORY)


%.o: %.cpp
	$(compilexx)

%.o: %.c
	$(compile)

%.o: %.S
	$(assemble)

clean:
	$(RM) $(OBJECTS_Vexriscv) glitch-pmp.elf glitch-pmp.bin .*~ *~

.PHONY: all clean
