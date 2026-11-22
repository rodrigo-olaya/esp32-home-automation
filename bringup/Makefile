# Bring-up programs for the ESP32 DevKit and the carrier board. Each folder is a tiny ESP-IDF project.
#
#   make id                    read chip and flash info over USB (changes nothing on the board)
#   make build-chip-check      build one program
#   make flash-chip-check      build, flash, then open the serial monitor (Ctrl+] to quit)
#   make monitor-chip-check    just open the serial monitor
#   make clean-chip-check
#
# PORT is the DevKit's USB serial port: `ls /dev/cu.*` with it plugged in.
IDF_EXPORT ?= $(HOME)/esp/v5.5/esp-idf/export.sh
PORT ?= /dev/cu.usbserial-0001

IDF = . $(IDF_EXPORT) > /dev/null && idf.py

PROGRAMS = chip-check i2c-scan sht31-read

help:
	@echo "programs: $(PROGRAMS)"
	@echo "make id | build-<name> | flash-<name> | monitor-<name> | clean-<name>   (PORT=$(PORT))"

id:
	. $(IDF_EXPORT) > /dev/null && esptool.py --port $(PORT) chip_id flash_id

build-%:
	$(IDF) -C $* build

flash-%:
	$(IDF) -C $* -p $(PORT) flash monitor

monitor-%:
	$(IDF) -C $* -p $(PORT) monitor

clean-%:
	$(IDF) -C $* fullclean

.PHONY: help id
