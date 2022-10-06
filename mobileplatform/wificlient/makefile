ESPTOOL = esptool.py
FFLAGS = --chip esp32 --port /dev/ttyUSB0
BFLAG = --baud 460800

clean:
	pio run --target clean

build:
	pio run

flash: clean
	pio run
	$(ESPTOOL) $(FFLAGS) $(BFLAG) write_flash --flash_size=detect 0 .pio/build/esp12e/firmware.bin

erase:
	$(ESPTOOL) $(FFLAGS) erase_flash