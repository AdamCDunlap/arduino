ARDUINO_DIR = /home/adam/arduino-unstable
ARDMK_DIR = /home/adam/patches/Arduino-Makefile
AVR_TOOLS_DIR = /usr
ARDUINO_SKETCHBOOK = /home/adam/arduino
AVRDUDE = $(ARDUINO_DIR)/hardware/tools/avrdude
AVRDUDE_CONF = $(ARDUINO_DIR)/hardware/tools/avrdude.conf

CPPFLAGS += -Os -Wall

#ifndef MONITOR_BAUDRATE
#	MONITOR_BAUDRATE = 115200
#endif

ifeq ($(wildcard /dev/ttyUSB*),)
	BOARD_TAG ?= uno
	ARDUINO_PORT ?= /dev/ttyACM*
else
	BOARD_TAG ?= atmega328
	ARDUINO_PORT ?= /dev/ttyUSB*
endif

ifndef ARDUINO_VERSION
	ARDUINO_VERSION = 101
endif

u: upload
m: monitor
um: u m
mu: u m
.DEFAULT_GOAL := all

include $(ARDMK_DIR)/arduino-mk/Arduino.mk

