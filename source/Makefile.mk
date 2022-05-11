
VPATH += ./dependencies/include
CFLAGS  += -I./dependencies/include

VPATH += ./source/ble
CFLAGS  += -I./source/ble

VPATH += ./source/ecdh
CFLAGS  += -I./source/ecdh


OBJ += main.o 
OBJ += ble_cli.o
OBJ += ble_evt_print.o
OBJ += ble_serial.o

OBJ += ecc_dh.o
OBJ += ecc.o
OBJ += utils.o
