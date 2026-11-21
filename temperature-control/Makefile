# Build and flash the sensor-node firmware. Same pattern as ../bringup/Makefile.
#
#   make build        compile
#   make flash        build, flash, then open the serial monitor (Ctrl+] to quit)
#   make monitor      just open the serial monitor
#   make menuconfig   ESP-IDF settings
#   make test         host-side unit tests (CppUTest), no board needed
#   make clean
#
# PORT is the DevKit's USB serial port: `ls /dev/cu.*` with it plugged in.
# Needs main/config/private_config.h (copy private_config.h.example and edit).
IDF_EXPORT ?= $(HOME)/esp/v5.5/esp-idf/export.sh
PORT ?= /dev/cu.usbserial-0001

IDF = . $(IDF_EXPORT) > /dev/null && idf.py

build:
	$(IDF) build

flash:
	$(IDF) -p $(PORT) flash monitor

monitor:
	$(IDF) -p $(PORT) monitor

menuconfig:
	$(IDF) menuconfig

test:
	$(MAKE) -C test test

clean:
	$(IDF) fullclean

.PHONY: build flash monitor menuconfig test clean
