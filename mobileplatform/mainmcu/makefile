BIN = arm-none-eabi-objcopy
STL = st-flash

clean:
	pio run --target clean

flash: clean
	pio run
	$(BIN) -O binary .pio/build/genericSTM32F103C8/firmware.elf app.bin
	$(STL) write app.bin 0x8000000

erase:
	$(STL) erase
