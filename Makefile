-include ./source/Makefile.mk

.PHONY: all clean

CFLAGS += -lpthread

MODUN=BleGW0


all: $(OBJ)
	@echo build cmd  $(OBJ)
	gcc -o $(MODUN) $^ $(CFLAGS) $(LIBS)

clean:
	@echo clean build
	rm -rf *.o $(MODUN)

