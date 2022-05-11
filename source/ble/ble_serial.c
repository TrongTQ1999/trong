#include "ble_cli.h"
#include "ecc_dh.h"
#include "ecc.h"
#include "ble_evt.h"
#include "ble_packet.h"
#include "ble_evt_print.h"
#include "ble_cmd.h"
#include "serial_status.h"
#include "ble_serial.h"
#include "ble_config_messages.h"

#include <stdio.h> // standard input / output functions
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

const uint8_t network_key[16] = {0x18, 0xee, 0xd9, 0xc2, 0xa5, 0x6a, 0xdd, 0x85, 0x04, 0x9f, 0xfc, 0x3c, 0x59, 0xad, 0x0e, 0x12};
/*Authentication data*/
const uint8_t authent_data[16] = {0x6E, 0x6F, 0x72, 0x64, 0x69, 0x63, 0x5F, 0x65, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x5F, 0x31};

/*AppKey Mesh*/
const uint8_t appKey[16] = {0x4f, 0x68, 0xad, 0x85, 0xd9, 0xf4, 0x8a, 0xc8, 0x58, 0x9d, 0xf6, 0x65, 0xb6, 0xb4, 0x9b, 0x8a};
const uint8_t appKey2[16] = {0x2a, 0xa2, 0xa6, 0xde, 0xd5, 0xa0, 0x79, 0x8c, 0xea, 0xb5, 0x78, 0x7c, 0xa3, 0xae, 0x39, 0xfc};

/*Data Authentication data*/
const uint8_t data_authencation[16] = {0x6E, 0x6F, 0x72, 0x64, 0x69, 0x63, 0x5F, 0x65, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x5F, 0x31};
uint16_t dev_addr = 0x001a;
static int ble_set_interface_attribs(int fd, int speed, int parity)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0)
    {
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0;        // no signaling chars, no echo,
                            // no canonical processing
    tty.c_oflag = 0;        // no remapping, no delays
    tty.c_cc[VMIN] = 0;     // read doesn't block
    tty.c_cc[VTIME] = 0.5;    // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);   // ignore modem controls,
                                       // enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        return -1;
    }

    return 0;
}

int ble_open()
{
    printf("BLE open port ...\n");
    int ret;
    int fd = open(BLE_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        fprintf(stderr, "error scan port\n");
        return -1;
    }
    printf("BLE open success\n");

    ret = ble_set_interface_attribs(fd, BLE_BR, 0);
    if(ret != 0)
    {
        printf("ble_set_interface_attribs ERROR \n");
        return -1;
    }

    return fd;
}

int ble_write(int fd, unsigned char opcode, unsigned char *payload, unsigned char size)
{
    int i;
    unsigned char buf[258];
    int len = 1 + size;
    buf[0] = len;
    buf[1] = opcode;
    
    if (size > 0)
    {
        memcpy(buf + 2, payload, size);
    }
    printf("BLE write: ");
    for (i = 0; i < len + 1; i++)
    {
        printf("%02x ", buf[i]);
    }
    printf("\n");
    return write(fd, buf, len + 1);
    // if == -1 wire fail (connect serial error.)
}

/* SERIAL COMMAND  */
bool ble_radioReset(int fd)
{
    if (ble_write(fd, SERIAL_OPCODE_CMD_DEVICE_RADIO_RESET, NULL, 0) == -1)
        return false;
    return true;
}

void ble_scanStart(int fd)
{
    ble_write(fd, SERIAL_OPCODE_CMD_DFU_JUMP_TO_BOOTLOADER, NULL, 0);
}

void ble_scanStop(int fd) 
{
    ble_write(fd, 0x62, NULL, 0);
}

void ble_provision(int fd, ble_ProvUnprovisionedRecv_t *provUnproRecv)
{
    unsigned char buffer[44];
    serial_cmd_prov_data_t provData;
    provData.context_id = 0x00; // Context ID use for this provisioning
    memcpy(provData.target_uuid, provUnproRecv->uuid, sizeof(provUnproRecv->uuid));
    memcpy(provData.network_key, network_key, sizeof(network_key));
    provData.network_key_index = netkey_id;
    provData.iv_index = 0x00000000; // Iv index of the network
    dev_addr = dev_addr + 1;
    provData.address = dev_addr; // Unicast address assigned to the device
    provData.iv_update_flag = 0x00;
    provData.key_refresh_flag = 0x00;
    provData.attention_duration_s = 0x00;

    memcpy(buffer, &provData, sizeof(provData));
    //buffer[12] = 0x0d;

    ble_write(fd, 0x63, buffer, sizeof(provData));
}

void ble_OOBuse(int fd, ble_ProvCapsRecv_t *provCapsRecv)
{
    unsigned char buffer[4];
    serial_cmd_prov_oob_use_t provOobUse;
    provOobUse.context_id = provCapsRecv->contextID;
    provOobUse.oob_method = 0x01; // OOB method use: Static OOB authentication method
    provOobUse.oob_action = *(uint8_t *)provCapsRecv->inOOBAct;
    provOobUse.size = provCapsRecv->inOOBSize;

    memcpy(buffer, &provOobUse, sizeof(provOobUse));

    ble_write(fd, 0x66, buffer, sizeof(provOobUse));
}

void ble_ECDHSecret(int fd, ble_ProvECDHReq_t *provECDHReq)
{
    unsigned char buffer[33];
    const struct uECC_Curve_t *curve = uECC_secp256r1();
    serial_cmd_prov_ecdh_data_t provEcdhData;

    provEcdhData.context_id = provECDHReq->contextID;
    uECC_shared_secret(provECDHReq->peerPublic, provECDHReq->nodePrivate, provEcdhData.shared_secret, curve);

    memcpy(buffer, &provEcdhData, sizeof(provEcdhData));

    ble_write(fd, 0x68, buffer, sizeof(provEcdhData));
}

void ble_AuthData(int fd, ble_ProvAuthReq_t *provAuthReq)
{
    unsigned char buffer[17];
    serial_cmd_prov_auth_data_t provAuthData;
    provAuthData.context_id = provAuthReq->contextID;
    memcpy(provAuthData.data, data_authencation, sizeof(data_authencation));

    memcpy(buffer, &provAuthData, sizeof(provAuthData));

    ble_write(fd, 0x67, buffer, sizeof(provAuthData));
}

void ble_AddrLocalUnicastSet(int fd)
{
    unsigned char buffer[4];
    serial_cmd_mesh_addr_local_unicast_set_t addrLocalUnicast;

    addrLocalUnicast.start_address = 0x0001; // First address in the range of unicast addresses.
    addrLocalUnicast.count = 0x0001;         // Number of addresses in the range of unicast addresses.

    memcpy(buffer, &addrLocalUnicast, sizeof(addrLocalUnicast));

    ble_write(fd, 0x9f, buffer, sizeof(addrLocalUnicast));
}

void ble_SubnetAdd(int fd)
{
    unsigned char buffer[18];
    serial_cmd_mesh_subnet_add_t subnetAdd;

    subnetAdd.net_key_index = netkey_id;
    memcpy(subnetAdd.key, network_key, sizeof(network_key));

    memcpy(buffer, &subnetAdd, sizeof(subnetAdd));

    ble_write(fd, 0x92, buffer, sizeof(subnetAdd));
}

void ble_AppKeyAdd(int fd, ble_unprovisioner_t *unprovisioner)
{
    unsigned char buffer[20];
    serial_cmd_mesh_appkey_add_t appKeyAdd;

    appKeyAdd.app_key_index = appKey_id;
    appKeyAdd.subnet_handle = unprovisioner->subnet_handle;
    memcpy(appKeyAdd.key, appKey, sizeof(appKey));

    memcpy(buffer, &appKeyAdd, sizeof(appKeyAdd));

    ble_write(fd, 0x97, buffer, sizeof(appKeyAdd));
}

void ble_AppKeyAdd2(int fd, ble_unprovisioner_t *unprovisioner)
{
    unsigned char buffer[20];
    serial_cmd_mesh_appkey_add_t appKeyAdd;

    appKeyAdd.app_key_index = appKey_id2;
    appKeyAdd.subnet_handle = unprovisioner->subnet_handle;
    memcpy(appKeyAdd.key, appKey2, sizeof(appKey2));

    memcpy(buffer, &appKeyAdd, sizeof(appKeyAdd));

    ble_write(fd, 0x97, buffer, sizeof(appKeyAdd));
}

void ble_DevkeyAdd(int fd, ble_ProvComplete_t *provComplete, ble_unprovisioner_t *unprovisioner)
{
    unsigned char buffer[20];
    serial_cmd_mesh_devkey_add_t devKeyAdd;

    devKeyAdd.owner_addr = dev_addr;
    devKeyAdd.subnet_handle = unprovisioner->subnet_handle;
    memcpy(devKeyAdd.key, provComplete->devkey, sizeof(provComplete->devkey));

    memcpy(buffer, &devKeyAdd, sizeof(devKeyAdd));

    ble_write(fd, 0x9c, buffer, sizeof(devKeyAdd));
}

void ble_AddrPublicationAdd(int fd)
{
    unsigned char buffer[2];
    serial_cmd_mesh_addr_publication_add_t addrPublicationAdd;

    addrPublicationAdd.address = dev_addr;

    memcpy(buffer, &addrPublicationAdd, sizeof(addrPublicationAdd));

    ble_write(fd, 0xa4, buffer, sizeof(addrPublicationAdd));
}

void ble_AddrSubcriptionAdd(int fd)
{
    unsigned char buffer[2];
    serial_cmd_mesh_addr_subscription_add_t addrSubcriptionAdd;

    addrSubcriptionAdd.address = dev_addr;
    memcpy(buffer, &addrSubcriptionAdd, sizeof(addrSubcriptionAdd));

    ble_write(fd, 0xa1, buffer, sizeof(addrSubcriptionAdd));
}

void ble_PackerSendCompositionData(int fd, ble_unprovisioner_t *unprovisioner)
{
    serial_cmd_mesh_packet_send_t packet;

    packet.appkey_handle = unprovisioner->devkey_handle;
    packet.src_addr = BLE_SRC_ADDR;
    packet.dst_addr_handle = unprovisioner->dst_addr_handle;
    packet.ttl = 0x08;                        // Time To Live value to use in packet.
    packet.force_segmented = 0x00;            // Whether or not to force use of segmented message type
    packet.transmic_size = 0x00;              // SMALL=0, LARGE=1, DEFAULT=2.
    packet.friendship_credential_flag = 0x00; // 0 for master, 1 for friendship
    packet.len = 3;
    packet.data[0] = 0x80;
    packet.data[1] = 0x08;
    packet.data[2] = 0x00;
    printf("ble_PackerSendCompositionData\n");
    ble_PacketSend(fd, &packet);
}

void ble_PackerSendAppkeyAdd(int fd, ble_unprovisioner_t *unprovisioner)
{
    serial_cmd_mesh_packet_send_t packet;

    packet.appkey_handle = unprovisioner->devkey_handle;
    packet.src_addr = BLE_SRC_ADDR;
    packet.dst_addr_handle = unprovisioner->dst_addr_handle;
    packet.ttl = 0x08;                        // Time To Live value to use in packet.
    packet.force_segmented = 0x00;            // Whether or not to force use of segmented message type
    packet.transmic_size = 0x00;              // SMALL=0, LARGE=1, DEFAULT=2.
    packet.friendship_credential_flag = 0x00; // 0 for master, 1 for friendship
    packet.len = 20;
    memset(&packet.data[0], 0, 4);
    memcpy(&packet.data[4], appKey, sizeof(appKey));

    ble_PacketSend(fd, &packet);
}

void ble_PacketSendModelAppBind(int fd, ble_unprovisioner_t *unprovisioner)
{
    serial_cmd_mesh_packet_send_t packet;

    packet.appkey_handle = unprovisioner->devkey_handle;
    packet.src_addr = BLE_SRC_ADDR;
    packet.dst_addr_handle = unprovisioner->dst_addr_handle;
    packet.ttl = 0x08;                        // Time To Live value to use in packet.
    packet.force_segmented = 0x00;            // Whether or not to force use of segmented message type
    packet.transmic_size = 0x00;              // SMALL=0, LARGE=1, DEFAULT=2.
    packet.friendship_credential_flag = 0x00; // 0 for master, 1 for friendship
    packet.len = 8;
    packet.data[0] = 0x80;
    packet.data[1] = 0x3d;
    packet.data[2] = 0x1a;
    packet.data[3] = 0x00;
    packet.data[4] = 0x00;
    packet.data[5] = 0x00;
    packet.data[6] = 0x00;
    packet.data[7] = 0x10;

    ble_PacketSend(fd, &packet);
}

void ble_PacketSendGenericClientSetTrue(int fd, ble_unprovisioner_t *unprovisioner)
{
    serial_cmd_mesh_packet_send_t packet;

    packet.appkey_handle = unprovisioner->subnet_handle;
    packet.src_addr = BLE_SRC_ADDR;
    packet.dst_addr_handle = unprovisioner->dst_addr_handle;
    packet.ttl = 0x08;                        // Time To Live value to use in packet.
    packet.force_segmented = 0x00;            // Whether or not to force use of segmented message type
    packet.transmic_size = 0x00;              // SMALL=0, LARGE=1, DEFAULT=2.
    packet.friendship_credential_flag = 0x00; // 0 for master, 1 for friendship
    packet.len = 4;
    packet.data[0] = 0x82;
    packet.data[1] = 0x02;
    packet.data[2] = 0x01;
    packet.data[3] = 0x01;

    ble_PacketSend(fd, &packet);
}

void ble_PacketSendGenericClientSetFalse(int fd, ble_unprovisioner_t *unprovisioner)
{
    serial_cmd_mesh_packet_send_t packet;

    packet.appkey_handle = unprovisioner->subnet_handle;
    packet.src_addr = BLE_SRC_ADDR;
    packet.dst_addr_handle = unprovisioner->dst_addr_handle;
    packet.ttl = 0x08;                        // Time To Live value to use in packet.
    packet.force_segmented = 0x00;            // Whether or not to force use of segmented message type
    packet.transmic_size = 0x00;              // SMALL=0, LARGE=1, DEFAULT=2.
    packet.friendship_credential_flag = 0x00; // 0 for master, 1 for friendship
    packet.len = 4;
    packet.data[0] = 0x82;
    packet.data[1] = 0x02;
    packet.data[2] = 0x00;
    packet.data[3] = 0x02;

    ble_PacketSend(fd, &packet);
}

void ble_PacketSendNodeReset(int fd, ble_unprovisioner_t *unprovisioner)
{
    serial_cmd_mesh_packet_send_t packet;

    packet.appkey_handle = unprovisioner->devkey_handle;
    packet.src_addr = BLE_SRC_ADDR;
    packet.dst_addr_handle = unprovisioner->dst_addr_handle;
    packet.ttl = 0x08;                        // Time To Live value to use in packet.
    packet.force_segmented = 0x00;            // Whether or not to force use of segmented message type
    packet.transmic_size = 0x00;              // SMALL=0, LARGE=1, DEFAULT=2.
    packet.friendship_credential_flag = 0x00; // 0 for master, 1 for friendship
    packet.len = 2;
    packet.data[0] = 0x80;
    packet.data[1] = 0x49;

    ble_PacketSend(fd, &packet);
}

void ble_PacketSendPublicationSet(int fd, ble_unprovisioner_t *unprovisioner)
{
    serial_cmd_mesh_packet_send_t packet;

    packet.appkey_handle = unprovisioner->devkey_handle;
    packet.src_addr = BLE_SRC_ADDR;
    packet.dst_addr_handle = unprovisioner->dst_addr_handle;
    packet.ttl = 0x08;                        // Time To Live value to use in packet.
    packet.force_segmented = 0x00;            // Whether or not to force use of segmented message type
    packet.transmic_size = 0x00;              // SMALL=0, LARGE=1, DEFAULT=2.
    packet.friendship_credential_flag = 0x00; // 0 for master, 1 for friendship
    packet.len = 12;
    packet.data[0] = 0x03;
    packet.data[1] = 0x1a;
    packet.data[2] = 0x00;
    packet.data[3] = 0x01;
    packet.data[4] = 0x00;
    packet.data[5] = 0x00;
    packet.data[6] = 0x00;
    packet.data[7] = 0x01;
    packet.data[8] = 0x00;
    packet.data[9] = 0x00;
    packet.data[10] = 0x00;
    packet.data[11] = 0x10;

    ble_PacketSend(fd, &packet);
}
