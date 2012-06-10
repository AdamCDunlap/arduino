ARDUINO_LIBS = 

BOARD_TAG    = uno

ifeq ($(BOARD_TAG),uno)
	ARDUINO_PORT = /dev/ttyACM0
else
	ARDUINO_PORT = /dev/tty
endif
ARDUINO_DIR = /home/adam/arduino-unstable
ARDMK_DIR = /home/adam/Arduino-Makefile
AVR_TOOLS_DIR = /usr
include $(ARDMK_DIR)/arduino-mk/Arduino.mk
