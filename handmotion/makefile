ESPTOOL = esptool.py
FFLAGS = --chip esp32 --port /dev/ttyUSB0
BFLAG = --baud 460800

clean:
	rm -f .pio/build/esp12e/firmware.*

flash: clean
	pio run
	$(ESPTOOL) $(FFLAGS) $(BFLAG) write_flash --flash_size=detect 0 .pio/build/esp12e/firmware.bin

erase:
	$(ESPTOOL) $(FFLAGS) erase_flash