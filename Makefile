# Project Name
TARGET = blink

# Sources
CPP_SOURCES = main.cpp midi.cpp pitch.cpp

# Library Locations
LIBDAISY_DIR = ./libDaisy
DAISYSP_DIR = ./DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile