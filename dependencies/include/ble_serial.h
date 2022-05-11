#ifndef __BLE_SERIAL_H__
#define __BLE_SERIAL_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "ble_evt.h"
#include "ble_cmd.h"
#include "serial_status.h"

#define BLE_PORT "/dev/ttyS4"
#define BLE_BR B115200

/*Address*/
#define BLE_SRC_ADDR 0x0001
// #define BLE_UNICAST 0x001a
/*Mesh-global key index*/
#define appKey_id 0x0000
#define appKey_id2 0x0001
#define netkey_id 0x0000

/* packet mesh define */
#define BLE_TTL_DEFAULT 0x04 // Time to live (not information in document nordic)



static int ble_set_interface_attribs(int fd, int speed, int parity);

// open fd
int ble_open();

// write data in uart port
int ble_write(int fd, unsigned char opcode, unsigned char *payload, unsigned char size);


// BLE command packet
bool ble_radioReset(int fd);
void ble_scanStart(int fd);
void ble_scanStop(int fd);
void ble_provision(int fd, ble_ProvUnprovisionedRecv_t *provUnproRecv);
void ble_OOBuse(int fd, ble_ProvCapsRecv_t *provCapsRecv);
void ble_ECDHSecret(int fd, ble_ProvECDHReq_t *provECDHReq);

void ble_SubnetAdd(int fd);
void ble_AppKeyAdd(int fd, ble_unprovisioner_t *unprovisioner);
void ble_AppKeyAdd2(int fd, ble_unprovisioner_t *unprovisioner);
void ble_AddrLocalUnicastSet(int fd);
void ble_AddrSubcriptionAdd(int fd);
void ble_AddrPublicationAdd(int fd);

void ble_PackerSendAppkeyAdd(int fd, ble_unprovisioner_t *unprovisioner);
void ble_DevkeyAdd(int fd, ble_ProvComplete_t *provComplete, ble_unprovisioner_t *unprovisioner);
void ble_PacketSend(int fd, serial_cmd_mesh_packet_send_t *packetSend);
void ble_PackerSendCompositionData(int fd, ble_unprovisioner_t *unprovisioner);
void ble_PacketSendModelAppBind(int fd, ble_unprovisioner_t *unprovisioner);
void ble_KeypairSet(int fd);

void ble_PacketSendGenericClientSetTrue(int fd, ble_unprovisioner_t *unprovisioner);
void ble_PacketSendGenericClientSetFalse(int fd, ble_unprovisioner_t *unprovisioner);

void ble_PacketSendNodeReset(int fd, ble_unprovisioner_t *unprovisioner);
void ble_PacketSendPublicationSet(int fd, ble_unprovisioner_t *unprovisioner);

void ble_AuthData(int fd, ble_ProvAuthReq_t *provAuthReq);

#endif /* __BLE_SERIAL_H__*/
