BIN = arm-none-eabi-objcopy
STL = st-flash

clean:
	pio run --target clean

build:
	pio run

all: clean build flash

flash: 
	$(BIN) -O binary .pio/build/genericSTM32F103C8/firmware_mainmcu_genericSTM32F103C8.elf app.bin
	$(STL) write app.bin 0x8000000

erase:
	$(STL) erase
