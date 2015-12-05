CC 	= avr-gcc
OBJCOPY	= avr-objcopy
OBJDUMP	= avr-objdump
SIZE 	= avr-size
AVRDUDE = avrdude

CCFLAGS += -mmcu=$(MCU)
CCFLAGS += -Os
CCFLAGS += -mtiny-stack
CCFLAGS += -fshort-enums
CCFLAGS += -flto
#CCFLAGS += -mcall-prologues

FUSE_DEFAULT = -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
#FUSE_RELEASE = -U lfuse:w:0x64:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

BUILD += $(MAIN).hex $(MAIN).eep

elf: $(MAIN).elf

%.elf: $(OBJECTS) muos/hw/$(PLATFORM)/platform.mk
	$(PRINTFMT) $@ [LINK]
	$(CC) $(LDFLAGS) $(OBJECTS) --output $@ 2>&1 | sed 'h;:b;$b;N;N;/appears to be a misspelled signal handler/{N;d};$b;D'

%.asm: %.elf
	echo "$@ \t←\t $< \t\t\t[ASM]"
	$(OBJDUMP) -S $< >$@

