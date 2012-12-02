ARDUINO_LIBS = LEDArray

BOARD_TAG    = uno

ifeq ($(BOARD_TAG),uno)
	ARDUINO_PORT = /dev/ttyACM0
else
	ARDUINO_PORT = /dev/tty
endif
ARDUINO_DIR = /home/adam/arduino-unstable
ARDMK_DIR = /home/adam/patches/Arduino-Makefile
AVR_TOOLS_DIR = /usr
ARDUINO_SKETCHBOOK = /home/adam/arduino
include $(ARDMK_DIR)/arduino-mk/Arduino.mk
