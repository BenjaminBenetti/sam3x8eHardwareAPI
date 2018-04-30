OBYCP = arm-none-eabi-objcopy
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
CFLAGS = --std=gnu11 -O0 -mcpu=cortex-m3 -mthumb -fno-builtin --specs=nosys.specs --specs=nano.specs -ffreestanding
INCLD = -I ../3rdParty/ATMEL/ -I ../3rdParty/CMSIS/include

OUTPUT_DIR = ../build
SRC_CC = contextSwitching/*.c UART/*.c Timers/*.c EEFC/*.c general/*.c

libsam3x8e.a : $(SRC_CC) $(OUTPUT_DIR)/.dirstamp
	$(CC) $(CFLAGS) -c $(SRC_CC) $(INCLD)
	mv ./*.o $(OUTPUT_DIR)
	$(AR) rcs ../build/$@ ../build/*.o

$(OUTPUT_DIR)/.dirstamp:
	mkdir -p $(OUTPUT_DIR)
	touch $@

clean:
	rm ../build/libsam3x8e.a
	rm ../build/*.o
