OBYCP = arm-none-eabi-objcopy
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
CFLAGS = --std=gnu11 -Os -mcpu=cortex-m3 -mthumb -fno-builtin  --specs=nosys.specs -ffreestanding
INCLD = -I ../3rdParty/ATMEL/ -I ../3rdParty/CMSIS/include

OUTPUT_DIR = ../build
SRC_CC = UART/*.c Timers/*.c EEFC/*.c
SRC_O  =  ../build/*.o


libsam3x8e.a : $(SRC_O)
	$(AR) rcs ../build/$@ $^

$(SRC_O) : $(SRC_CC) $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -c $(SRC_CC) $(INCLD)
	mv ./*.o $(OUTPUT_DIR)

$(OUTPUT_DIR):
	mkdir $(OUTPUT_DIR)

clean:
	rm ../build/libsam3x8e.a
	rm ../build/*.o
