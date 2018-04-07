OBYCP = arm-none-eabi-objcopy
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
CFLAGS = --std=gnu11 -Os -mcpu=cortex-m3 -mthumb -fno-builtin  --specs=nosys.specs -ffreestanding
INCLD = -I ../../../tools/ATMEL/ -I ../../../tools/CMSIS/include

SRC_CC = UART/*.c Timers/*.c EEFC/*.c
SRC_O  =  ../build/*.o


libsam3x8e.a : $(SRC_O)
	$(AR) rcs ../build/$@ $^

$(SRC_O) : $(SRC_CC)
	$(CC) $(CFLAGS) -c $^ $(INCLD)
	mv ./*.o ../build/

clean:
	rm ../build/libsam3x8e.a
	rm ../build/*.o
