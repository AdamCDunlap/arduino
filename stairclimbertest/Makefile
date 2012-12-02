ARDUINO_LIBS = 



BOARD_TAG    = uno



ARDUINO_SKETCHBOOK = /home/adam/arduino
ARDUINO_DIR = /home/adam/arduino-unstable
ifeq ($(BOARD_TAG),uno)
	ARDUINO_PORT = /dev/ttyACM*
else
	ARDUINO_PORT = /dev/ttyUSB*
endif

AVR_TOOLS_PATH = /usr/bin
include /usr/share/arduino-mk/Arduino.mk
