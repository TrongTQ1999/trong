
#include "ble_cli.h"
#include "ecc_dh.h"
#include "ecc.h"
#include "ble_evt.h"
#include "ble_packet.h"
#include "ble_evt_print.h"
#include "ble_cmd.h"
#include "serial_status.h"
#include "ble_serial.h"
#include "config_opcodes.h"
#include "generic_onoff_messages.h"

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

// #define BLE_PORT "/dev/ttyS3"
// #define BLE_BR B115200

#define SERIAL_PACKET_OVERHEAD 19

ble_unprovisioner_t unprovisioner;

// Ble Receive
typedef enum
{
    BLE_MESH_PARSE_STATE_LEN,
    BLE_MESH_PARSE_STATE_OPCODE,
    BLE_MESH_PARSE_STATE_DATA,
} BLE_MESH_PARSE_STATE_E;

typedef struct
{
    unsigned char opcode;
    int len;

    unsigned char data[256];
    union
    {
        void *data;
        ble_ProvUnprovisionedRecv_t *provUnprovisioned;
        ble_ProvECDHReq_t *provECDHReq;
        ble_ProvLinkEstablish_t *provlinkEstablish;
        ble_ProvCapsRecv_t *provCapsRecv;
        ble_ProvAuthReq_t *provAuthReq;
        ble_ProvComplete_t *provComplete;
        ble_ProvLinkCl_t *provLinkCl;
        ble_CmdRsp_t *cmdRsp;
        serial_evt_mesh_message_received_t *meshMessageRec;
        serial_evt_device_started_t *deviceStarted;
        serial_evt_mesh_iv_entry_set_notification_t *ivEntrySetNoti;
        serial_evt_mesh_seqnum_entry_set_notification_t *seqnumEntrySetNoti;
        serial_evt_mesh_tx_complete_t *txComplete;
        serial_evt_prov_failed_t *provFailed;
    } this;

} ble_cmd_t;

int ble_start()
{
    pthread_t thread;
    int fd;

    fd = ble_open(); // check open status
    if (fd <= 0)
    {
        printf("open BLE error [%s]\n", strerror(errno));
        return -1;
    }

    if (!ble_radioReset(fd))
    {
        printf("open BLE radio reset error [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }

    sleep(2); // sleep 2s for receive reset

    if (pthread_create(&thread, NULL, ble_receive_handle, (void *)&fd) != 0)
    {

        printf("Error thread...\n");
        return -1;
    }

    printf("BLE write radio reset success \n");

    return fd;
}

typedef struct ble_parser_t
{
    unsigned char state; // BLE_MESH_PARSE_STATE_E
    ble_cmd_t cmd;
    int pos;
} ble_parser_t;

ble_parser_t localParser;

void ble_cli_init()
{
    memset(&localParser, 0, sizeof(localParser));
    localParser.cmd.this.data = (void *)localParser.cmd.data;
}

// Ble Send Packet
bool ble_parser(unsigned char *data, int size, ble_parser_t *parser, int *remainSize)
{
    unsigned char *p = data;

    while (size > 0)
    {
        switch (parser->state)
        {
        case BLE_MESH_PARSE_STATE_LEN:
        {
            // printf("BLE_MESH_PARSE_STATE_LEN\n");
            parser->pos = 0;
            parser->cmd.len = *p - 1;
            parser->state = BLE_MESH_PARSE_STATE_OPCODE;
            size--;
            p++;
        }
        break;
        case BLE_MESH_PARSE_STATE_OPCODE:
        {
            // printf("BLE_MESH_PARSE_STATE_OPCODE\n");
            parser->cmd.opcode = *p;
            parser->state = BLE_MESH_PARSE_STATE_DATA;
            size--;
            p++;
        }
        break;
        case BLE_MESH_PARSE_STATE_DATA:
        {
            // printf("BLE_MESH_PARSE_STATE_DATA\n");
            int rsize = parser->cmd.len - parser->pos;

            if (size >= rsize)
            {
                memcpy(parser->cmd.data + parser->pos, p, rsize);
                parser->pos += rsize;

                p += rsize;
                size -= rsize;

                // remain buffer not parser
                parser->state = BLE_MESH_PARSE_STATE_LEN;

                // status true
                *remainSize = size;
                // printf("Parser done!!!\n");
                return true;
            }
            else
            {
                memcpy(parser->cmd.data + parser->pos, p, size);
                parser->pos += size;
                size = 0;
            }
        }
        break;
        default:

            break;
        }
    }

    *remainSize = size;
    return false;
}

void ble_processDataCmdRsp(int fd)
{
    switch (localParser.cmd.this.cmdRsp->opcode)
    {
    case SERIAL_OPCODE_CMD_MESH_DEVKEY_ADD:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
            if (unprovisioner.state == BLE_UNPROVISIONER_STATE_DEVKEY_ADDRPUBADD)
            {
                unprovisioner.devkey_handle = *(uint16_t *)localParser.cmd.this.cmdRsp->data;
            }
            else
            {
                printf("\033[1;31mBLe add devkey error\n\300[0m");
            }
        }
    }
    break;

    case SERIAL_OPCODE_CMD_MESH_SUBNET_ADD:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
            if (unprovisioner.state == BLE_UNPROVISIONER_STATE_NEW)
            {
                unprovisioner.subnet_handle = *(uint16_t *)localParser.cmd.this.cmdRsp->data;
            }
            printf("\n\033[1;32mBLE AppKey Add \033[0m\n\n");
            ble_AppKeyAdd(fd, &unprovisioner);
            ble_AppKeyAdd2(fd, &unprovisioner);
        }
    }
    break;

    case SERIAL_OPCODE_CMD_MESH_ADDR_PUBLICATION_ADD:
    {
        printf("\n\033[1;32mSERIAL_OPCODE_CMD_MESH_ADDR_PUBLICATION_ADD \033[0m\n\n");
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
            if (unprovisioner.state == BLE_UNPROVISIONER_STATE_DEVKEY_ADDRPUBADD)
            {
                unprovisioner.dst_addr_handle = *(uint16_t *)localParser.cmd.this.cmdRsp->data;
            }
        }
    }
    break;

    case SERIAL_OPCODE_CMD_MESH_ADDR_LOCAL_UNICAST_SET:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
            printf("\n\033[1;32mBLE Subnet Add\033[0m\n\n");
            ble_SubnetAdd(fd);
        }
    }
    break;

    case SERIAL_OPCODE_CMD_MESH_APPKEY_ADD:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
        }
    }
    break;

    case SERIAL_OPCODE_CMD_PROV_SCAN_START:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
        }
    }
    break;

    case SERIAL_OPCODE_CMD_PROV_SCAN_STOP:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
        }
    }
    break;

    case SERIAL_OPCODE_CMD_PROV_PROVISION:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
        }
    }
    break;

    case SERIAL_OPCODE_CMD_PROV_OOB_USE:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
        }
    }
    break;

    case SERIAL_OPCODE_CMD_PROV_ECDH_SECRET:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
        }
    }
    break;

    case SERIAL_OPCODE_CMD_MESH_PACKET_SEND:
    {
        if (localParser.cmd.this.cmdRsp->status == SERIAL_STATUS_SUCCESS)
        {
            print_CmdRsp(localParser.cmd.this.cmdRsp, localParser.cmd.len - 2);
        }
    }
    break;

    default:
        printf("\033[1;31mBLE opcode response not suport [%02x]\n[\033[0m", localParser.cmd.this.cmdRsp->opcode);
        break;
    }
}

void ble_processDataMessageUnicast(int fd, uint16_t opcode, uint8_t *pData)
{
    switch (opcode)
    {
    case CONFIG_OPCODE_COMPOSITION_DATA_STATUS:
    {
        print_MsgCompositionDataStatus((config_msg_composition_data_status_t *)pData);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_PS_COMPOSDATA)
        {
            printf("\n\033[1;32mBLE Step 9: Packet Send Appkey Add\033[0m\n\n");
            ble_PackerSendAppkeyAdd(fd, &unprovisioner);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_PS_APPKEY_ADD;
        }
    }
    break;

    case CONFIG_OPCODE_APPKEY_STATUS:
    {
        print_MsgAppkeyStatus((config_msg_appkey_status_t *)pData);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_PS_APPKEY_ADD)
        {
            printf("\n\033[1;32mBLE Step 10: Packet Send Model App Bind\033[0m\n\n");
            ble_PacketSendModelAppBind(fd, &unprovisioner);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_PS_MODEL_APP_BIND;
        }
    }
    break;

    case CONFIG_OPCODE_MODEL_APP_STATUS:
    {
        config_msg_app_status_t *data;
        data = (config_msg_app_status_t *)pData;
        print_MsgAppStatus((config_msg_app_status_t *)pData);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_PS_MODEL_APP_BIND)
        {
            printf("\n\033[1;32mBLE Step 11: Packet Send Model Publication Set\033[0m\n\n");
            ble_PacketSendPublicationSet(fd, &unprovisioner);
            unprovisioner.element_address = data->element_address;
            printf("unprovisioner element: %04x\n", unprovisioner.element_address);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_PS_MODEL_PUBLICATION_SET;
        }
    }
    break;

    case CONFIG_OPCODE_MODEL_PUBLICATION_STATUS:
    {
        print_MsgPublicationStatus((config_msg_publication_status_t *)pData);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_PS_MODEL_PUBLICATION_SET)
        {
            unprovisioner.state = BLE_UNPROVISIONER_STATE_NEW;
        }
    }
    break;

    case GENERIC_ONOFF_OPCODE_STATUS:
    {
        if (*(uint8_t *)pData == 0x01)
        {
            printf("\n\033[1;32mGennericOnOffClient: Present OnOff: OFF\033[0m\n\n");
        }
        else
        {
            printf("\n\033[1;32mGennericOnOffClient: ON\033[0m\n\n");
        }
    }
    break;

    case CONFIG_OPCODE_NODE_RESET_STATUS:
    {
        printf("\n\033[1;32mRESET NODE\033[0m\n\n");
    }
    break;

    default:
        break;
    }
}

void ble_processCommand(int fd)
{
    switch (localParser.cmd.opcode)
    {
    case SERIAL_OPCODE_EVT_CMD_RSP:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_CMD_RSP \033[0m\n");
        ble_processDataCmdRsp(fd);
    }
    break;

    case SERIAL_OPCODE_EVT_PROV_UNPROVISIONED_RECEIVED:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_PROV_UNPROVISIONED_RECEIVED \033[0m\n");
        print_ProvUnprovisioned(localParser.cmd.this.provUnprovisioned);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_NEW)
        {

            printf("\n\033[1;32mBLE Step 2: Provisioning \033[0m\n\n");
            ble_provision(fd, localParser.cmd.this.provUnprovisioned);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_PROVISION;
        }
    }
    break;

    case SERIAL_OPCODE_EVT_PROV_LINK_ESTABLISHED:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_PROV_LINK_ESTABLISHED \033[0m\n");
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_PROVISION)
        {
            print_ProvLinkEsta(localParser.cmd.this.provlinkEstablish);
        }
    }
    break;

    case SERIAL_OPCODE_EVT_PROV_CAPS_RECEIVED:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_PROV_CAPS_RECEIVED \033[0m\n");
        print_ProvCapsRecv(localParser.cmd.this.provCapsRecv);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_PROVISION)
        {
            printf("\n\033[1;32mBLE Step 3: Provisioning OOB Use \033[0m\n\n");
            ble_OOBuse(fd, localParser.cmd.this.provCapsRecv);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_OOBUSE;
        }
    }
    break;

    case SERIAL_OPCODE_EVT_PROV_COMPLETE:
    {
        ble_scanStop(fd);
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_PROV_COMPLETE \033[0m\n");
        print_ProvComplete(localParser.cmd.this.provComplete);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_AUTH)
        {
            unprovisioner.dst_addr_handle = *(uint16_t *)localParser.cmd.this.provComplete->address;
            printf("\n\033[1;32mBLE Step 6: DevKey Add\033[0m\n\n");
            ble_DevkeyAdd(fd, localParser.cmd.this.provComplete, &unprovisioner);
            printf("\n\033[1;32mBLE Step 7: Addr Publication Add\033[0m\n\n");
            ble_AddrPublicationAdd(fd);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_DEVKEY_ADDRPUBADD;
            
        }
    }
    break;

    case SERIAL_OPCODE_EVT_PROV_ECDH_REQUEST:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_PROV_ECDH_REQUEST \033[0m\n");
        print_ProvECDHReq(localParser.cmd.this.provECDHReq);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_OOBUSE)
        {
            printf("\n\033[1;32mBLE Step 4: Provisioning ECDH \033[0m\n\n");
            ble_ECDHSecret(fd, localParser.cmd.this.provECDHReq);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_ECDH;
        }
    }
    break;

    case SERIAL_OPCODE_EVT_PROV_AUTH_REQUEST:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_PROV_AUTH_REQUEST \033[0m\n");
        print_ProvAuthReq(localParser.cmd.this.provAuthReq);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_ECDH)
        {
            printf("\n\033[1;32mBLE Step 5: Provisioning Auth Data \033[0m\n\n");
            ble_AuthData(fd, localParser.cmd.this.provAuthReq);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_AUTH;
        }
    }
    break;

    case SERIAL_OPCODE_EVT_PROV_LINK_CLOSED:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_PROV_LINK_CLOSED \033[0m\n");
        print_LinkClose(localParser.cmd.this.provLinkCl);
        printf("\n\033[1;32m...LINK CLOSE... \033[0m\n\n");
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_DEVKEY_ADDRPUBADD)
        {
            printf("\n\033[1;32mBLE Step 8: Packet Send Composition Data\033[0m\n\n");
            ble_PackerSendCompositionData(fd, &unprovisioner);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_PS_COMPOSDATA;
        }
        else
        {
            unprovisioner.state = BLE_UNPROVISIONER_STATE_NEW;
        }
    }
    break;

    case SERIAL_OPCODE_EVT_MESH_MESSAGE_RECEIVED_UNICAST:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_MESH_MESSAGE_RECEIVED_UNICAST \033[0m\n");
        print_MeshMessageRec(localParser.cmd.this.meshMessageRec, (localParser.cmd.len - SERIAL_PACKET_OVERHEAD));

        uint16_t opcode;
        uint8_t *pData;
        if (localParser.cmd.this.meshMessageRec->data[0] == 0x80)
        {
            opcode = localParser.cmd.this.meshMessageRec->data[1] | 0x8000;
            pData = localParser.cmd.this.meshMessageRec->data + 2;
            printf("\033[2;36mBLE receive unicast opcode [%04X]\n\n\033[0m", opcode);
        }
        else if (localParser.cmd.this.meshMessageRec->data[0] == 0x82)
        {
            opcode = localParser.cmd.this.meshMessageRec->data[1] | 0x8200;
            pData = localParser.cmd.this.meshMessageRec->data + 2;
            printf("\033[2;36mBLE receive unicast opcode [%04X]\n\n\033[0m", opcode);
        }
        else
        {
            opcode = localParser.cmd.this.meshMessageRec->data[0];
            pData = localParser.cmd.this.meshMessageRec->data + 1;
            printf("\033[2;36mBLE receive unicast opcode [%02X]\n\n\033[0m", opcode);
        }

        ble_processDataMessageUnicast(fd, opcode, pData);
    }
    break;

    case SERIAL_OPCODE_EVT_DEVICE_STARTED:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_DEVICE_STARTED \033[0m\n");
        print_deviceStarted(localParser.cmd.this.deviceStarted);
        printf("\n\033[1;32mBLE Addr Local unicast Set\033[0m\n\n");
        ble_AddrLocalUnicastSet(fd);
    }
    break;

    case SERIAL_OPCODE_EVT_MESH_IV_ENTRY_SET_NOTIFICATION:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_MESH_IV_ENTRY_SET_NOTIFICATION \033[0m\n");
        print_MeshIvEntrySetNoti(localParser.cmd.this.ivEntrySetNoti);
    }
    break;

    case SERIAL_OPCODE_EVT_MESH_SEQNUM_ENTRY_SET_NOTIFICATION:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_MESH_SEQNUM_ENTRY_SET_NOTIFICATION \033[0m\n");
        print_MeshSeqnumEntrySetNoti(localParser.cmd.this.seqnumEntrySetNoti);
    }
    break;

    case SERIAL_OPCODE_EVT_MESH_TX_COMPLETE:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_MESH_TX_COMPLETE \033[0m\n");
        print_MeshTXComplete(localParser.cmd.this.txComplete);
        if (unprovisioner.state == BLE_UNPROVISIONER_STATE_PS_COMPOSDATA)
        {
            printf("\n\033[1;32mBLE Step 9: Packet Send Appkey Add\033[0m\n\n");
            ble_PackerSendAppkeyAdd(fd, &unprovisioner);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_PS_APPKEY_ADD;
        }
    }
    break;

    case SERIAL_OPCODE_EVT_PROV_FAILED:
    {
        printf("\n\033[1;31m SERIAL_OPCODE_EVT_PROV_FAILED \033[0m\n");
        print_ProvFailed(localParser.cmd.this.provFailed);
    }
    break;

    default:
        printf("\033[1;31m BLE opcode not suport [%02x] \033[0m\n\n", localParser.cmd.opcode);
        break;
    }
}

static void ble_handle_cmd(int fd, unsigned char *data, int size)
{
    int remainSize = size;
    while (remainSize)
    {
        // printf("\rremainSize: %d\n", remainSize);
        if (ble_parser(data + size - remainSize, remainSize, &localParser, &remainSize))
        {
            printf("\033[1;33m\nparser cmd success opcode [%02X] len [%d] ++++++++++++++++++++\033[0m\n", localParser.cmd.opcode, localParser.cmd.len);
            ble_processCommand(fd);
        }
    }
}

void *ble_receive_handle(void *arg)
{
    int i;
    int fd = *(int *)arg;
    unsigned char buf[256];
    int len;
    while (1)
    {

        len = read(fd, buf, sizeof(buf));
        // datlt20
        if (len == 1 && buf[0] == 0)
        {
            printf("NRF Reinstall UART!!! Maybe device just restarted!!!\n");
            continue;
        }

        if (len > 0)
        {
            printf("BLE receive: ");
            for (i = 0; i < len; i++)
            {
                printf("%02x ", buf[i]);
            }
            printf("\n");

            ble_handle_cmd(fd, buf, len);
        }
    }
}

void ble_PacketSend(int fd, serial_cmd_mesh_packet_send_t *packetSend)
{

    unsigned char buffer[255];
    memcpy(buffer, packetSend, 10);

    memcpy(&buffer[10], packetSend->data, packetSend->len);

    ble_write(fd, 0xab, buffer, 10 + packetSend->len);
}
