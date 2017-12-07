BUILD_HOME = ../../../

include $(BUILD_HOME)/src/Makefile.include

LIBRARY_NAME = led
LIB = lib$(LIBRARY_NAME).a

SOURCE_FILES = led.c
OBJECT_FILES = ${SOURCE_FILES:.c=.o}

all: $(LIB)

$(LIB): $(OBJECT_FILES)
	$(AR) r $@ $(OBJECT_FILES)
