ARDUINO_LIBS = Servo
MONITOR_BAUDRATE = 115200

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
AVRDUDE = $(ARDUINO_DIR)/hardware/tools/avrdude
AVRDUDE_CONF = $(ARDUINO_DIR)/hardware/tools/avrdude.conf
include $(ARDMK_DIR)/arduino-mk/Arduino.mk
