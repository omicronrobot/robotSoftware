BIN = arm-none-eabi-objcopy
STL = st-flash

clean:
	rm -f .pio/build/esp12e/firmware.elf *.bin

flash: clean
	pio run
	$(BIN) -O binary .pio/build/esp12e/firmware.elf app.bin
	$(STL) write app.bin 0x8000000

erase:
	$(STL) erase
