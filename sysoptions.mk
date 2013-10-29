ARDUINO_DIR        ?= /usr/share/arduino
ARDMK_DIR          ?= /home/adam/src/Arduino-Makefile
AVR_TOOLS_DIR      ?= /usr
ARDUINO_SKETCHBOOK ?= /home/adam/docs/arduino
AVRDUDE            ?= $(ARDUINO_DIR)/hardware/tools/avrdude
AVRDUDE_CONF       ?= $(ARDUINO_DIR)/hardware/tools/avrdude.conf

CPPFLAGS += -Os -Wall -Wextra

#ARDUINO_PORT ?= /dev/ttyUSB*
ifeq ($(wildcard /dev/ttyUSB*),)
	BOARD_TAG ?= uno
	ARDUINO_PORT ?= /dev/ttyACM*
else
	BOARD_TAG ?= atmega328
	ARDUINO_PORT ?= /dev/ttyUSB*
endif

ARDUINO_VERSION ?= 103

u: upload
m: monitor
um: u m
mu: u m
.DEFAULT_GOAL := all

include $(ARDMK_DIR)/arduino-mk/Arduino.mk
