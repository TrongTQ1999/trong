#include "ble_evt_print.h"
#include "ble_evt.h"
#include "ble_config_messages.h"

#include <stdio.h>
#include <stdlib.h>

#define NUMBER_MODEL_ID 6

void print_ProvUnprovisioned(ble_ProvUnprovisionedRecv_t *provUnprovisioned)
{
    int i;
    printf("\n\033[1;32mBLE Step 1: Unprovisioned\033[0m\n\n");
    printf("BLE new Prov Unprovisioned Received: UUID 0x");
    for (i = 0; i < 16; i++)
    {
        printf("%02x", provUnprovisioned->uuid[i]);
    }
    printf("\n");
    printf("BLE new Prov Unprovisioned Received: rssi [%d]\n", provUnprovisioned->rssi);
    printf("BLE new Prov Unprovisioned Received: Gatt Support [%02x]\n", provUnprovisioned->gattSupport);
    printf("BLE new Prov Unprovisioned Received: Adv Addr Type [%02x]\n", provUnprovisioned->advAddrType);
    printf("BLE new Prov Unprovisioned Received: Adv Addr 0x");
    for (i = 0; i < 6; i++)
    {
        printf("%02x", provUnprovisioned->advAddr[i]);
    }
    printf("\n");
    printf("\n");
}

void print_ProvLinkEsta(ble_ProvLinkEstablish_t *provLinkEsta)
{
    printf("BLE new Prov Link Established: Context ID 0x%02x\n", provLinkEsta->contextID);
    printf("\n");
}

void print_ProvCapsRecv(ble_ProvCapsRecv_t *provCapsRecv)
{
    int i;
    printf("BLE new Prov Caps Received: Context ID 0x%02x\n", provCapsRecv->contextID);
    printf("BLE new Prov Caps Received: Num Element [%d]\n", provCapsRecv->num_element);
    printf("BLE new Prov Caps Received: Public Key Type 0x%02x\n", provCapsRecv->publicKeyType);
    printf("BLE new Prov Caps Received: Static OOB Types 0x%02x\n", provCapsRecv->staticOOBTypes);
    printf("BLE new Prov Caps Received: Output OOB Size 0x%02x\n", provCapsRecv->outOOBSize);
    printf("BLE new Prov Caps Received: Output OOB Actions 0x");
    for (i = 0; i < 2; i++)
    {
        printf("%02x", provCapsRecv->outOOBAct[i]);
    }
    printf("\n");
    printf("BLE new Prov Caps Received: Input OOB Size 0x%02x\n", provCapsRecv->inOOBSize);
    printf("BLE new Prov Caps Received: Input OOB Actions 0x");
    for (i = 0; i < 2; i++)
    {
        printf("%02x", provCapsRecv->inOOBAct[i]);
    }
    printf("\n");
    printf("\n");
}

void print_ProvECDHReq(ble_ProvECDHReq_t *provECDHReq)
{
    int i;
    printf("BLE new Prov ECDH Request: Context ID: [0x%02x]\n", provECDHReq->contextID);
    printf("BLE new Prov ECDH Request: Peer Public : ");
    for (i = 0; i < 64; i++)
    {
        printf("%02x ", provECDHReq->peerPublic[i]);
    }
    printf("\n");
    printf("BLE new Prov Unprovisioned Received: Node private : ");
    for (i = 0; i < 32; i++)
    {
        printf("%02x ", provECDHReq->nodePrivate[i]);
    }
    printf("\n");
    printf("\n");
}

void print_ProvAuthReq(ble_ProvAuthReq_t *provAuthReq)
{

    printf("BLE new Prov Auth Request: Context ID 0x%02x\n", provAuthReq->contextID);
    printf("BLE new Prov Auth Request: Method [%02x]\n", provAuthReq->method);
    printf("BLE new Prov Auth Request: Action 0x%02x\n", provAuthReq->action);
    printf("BLE new Prov Auth Request: Size 0x%02x\n", provAuthReq->size);
    printf("\n");
}

void print_ProvComplete(ble_ProvComplete_t *provComplete)
{
    int i;
    printf("BLE new Prov Complete: Context ID 0x%02x\n", provComplete->contextID);
    printf("BLE new Prov Complete: Iv Index 0x");
    for (i = 0; i < 4; i++)
    {
        printf("%02x", provComplete->iv_index[i]);
    }
    printf("\n");
    printf("BLE new Prov Complete: Net Key Index 0x");
    for (i = 0; i < 2; i++)
    {
        printf("%02x", provComplete->netkey_id[i]);
    }
    printf("\n");
    printf("BLE new Prov Complete: Address 0x");
    for (i = 0; i < 2; i++)
    {
        printf("%02x", provComplete->address[i]);
    }
    //  printf("%04x", provComplete->address);
    printf("\n");
    printf("BLE new Prov Complete: Iv Update Flag 0x%02x\n", provComplete->iv_update_flag);
    printf("BLE new Prov Complete: Key Refresh Flag [%02x]\n", provComplete->key_refre_flag);
    printf("BLE new Prov Complete: Device Key 0x");
    for (i = 0; i < 16; i++)
    {
        printf("%02x", provComplete->devkey[i]);
    }
    printf("\n");
    printf("BLE new Prov Complete: Net Key 0x");
    for (i = 0; i < 16; i++)
    {
        printf("%02x", provComplete->netkey[i]);
    }
    printf("\n");
    printf("\n");
}

void print_LinkClose(ble_ProvLinkCl_t *provLinkClo)
{
    printf("BLE new Prov Link Closed: Context ID 0x%02x\n", provLinkClo->contextID);
    printf("BLE new Prov Link Closed: Close Reason [%02x]\n", provLinkClo->closeRea);
    printf("\n");
}

void print_CmdRsp(ble_CmdRsp_t *cmdRsp, int size_data)
{
    int i;
    printf("BLE new Command Respone: Opcode 0x%02x\n", cmdRsp->opcode);
    printf("BLE new Command Respone: status 0x%02x\n", cmdRsp->status);
    printf("BLE new Command Respone: data 0x");
    for (i = 0; i < sizeof(size_data); i++)
    {
        printf("%02x ", cmdRsp->data[i]);
    }
    printf("\n");
    printf("\n");
}

void print_MeshMessageRec(serial_evt_mesh_message_received_t *meshMessageRec, int size_data)
{
    int i;
    printf("BLE new Mesh Message Receive: SRC 0x%04x\n", meshMessageRec->src);
    printf("BLE new Mesh Message Receive: DST 0x%04x\n", meshMessageRec->dst);
    printf("BLE new Mesh Message Receive: AppKey Handle 0x%04x\n", meshMessageRec->appkey_handle);
    printf("BLE new Mesh Message Receive: Subnet Handle 0x%04x\n", meshMessageRec->subnet_handle);
    printf("BLE new Mesh Message Receive: TTL 0x%02x\n", meshMessageRec->ttl);
    printf("BLE new Mesh Message Receive: Adv Addr Type 0x%02x\n", meshMessageRec->adv_addr_type);
    printf("BLE new Mesh Message Receive: Adv Addr 0x");
    for (i = 0; i < sizeof(meshMessageRec->adv_addr); i++)
    {
        printf("%02x ", meshMessageRec->adv_addr[i]);
    }
    printf("\n");
    printf("BLE new Mesh Message Receive: Rssi 0x%02x\n", meshMessageRec->rssi);
    printf("BLE new Mesh Message Receive: Actual Length 0x%04x\n", meshMessageRec->actual_length);
    printf("BLE new Mesh Message Receive: Data 0x");
    for (i = 0; i < size_data; i++)
    {
        printf("%02x ", meshMessageRec->data[i]);
    }
    printf("\n");
    printf("\n");
}

void print_deviceStarted(serial_evt_device_started_t *deviceStarted)
{
    printf("BLE new Device Started: Operating Mode 0x%02x\n", deviceStarted->operating_mode);
    printf("BLE new Device Started: HW Error 0x%02x\n", deviceStarted->hw_error);
    printf("BLE new Device Started: Data Credit Available 0x%02x\n", deviceStarted->data_credit_available);
    printf("\n");
}

void print_MeshIvEntrySetNoti(serial_evt_mesh_iv_entry_set_notification_t *ivEntruSetNoti)
{
    printf("BLE new Mesh IV Entry Set Notification: Iv Index 0x%08x\n", ivEntruSetNoti->iv_index);
    printf("BLE new Mesh IV Entry Set Notification: Iv Update In Progress 0x%02x\n", ivEntruSetNoti->iv_update_in_progress);
    printf("BLE new Mesh IV Entry Set Notification: Iv Update Timeout Counter 0x%04x\n", ivEntruSetNoti->iv_update_timout_counter);
    printf("\n");
}

void print_MeshSeqnumEntrySetNoti(serial_evt_mesh_seqnum_entry_set_notification_t *seqnumSetNoti)
{
    printf("BLE new Mesh Seqnum Entry Set Notification: Next Block 0x%08x\n", seqnumSetNoti->next_block);
    printf("\n");
}

void print_MeshTXComplete(serial_evt_mesh_tx_complete_t *txComplete)
{
    printf("BLE new Mesh TX Complete: Token 0x%08x\n", txComplete->token);
    printf("\n");
}

void print_ProvFailed(serial_evt_prov_failed_t *provFailed)
{
    printf("BLE new Prov Failed: Context ID 0x%02x\n", provFailed->context_id);
    printf("BLE new Prov Failed: Error Code 0x%02x\n", provFailed->error_code);
    printf("\n");
}

/* Print Messages Received Unicast */

void print_MsgCompositionDataStatus(config_msg_composition_data_status_t *msgCompositionDataStatus)
{
    int i;
    printf("BLE MSG Composotion Data Status: Page Number 0x%02x\n", msgCompositionDataStatus->page_number);
    printf("BLE MSG Composotion Data Status: Company id 0x%04x\n", msgCompositionDataStatus->cid);
    printf("BLE MSG Composotion Data Status: Product id 0x%04x\n", msgCompositionDataStatus->pid);
    printf("BLE MSG Composotion Data Status: Version id 0x%04x\n", msgCompositionDataStatus->vid);
    printf("BLE MSG Composotion Data Status: Replay Cache Entries 0x%04x\n", msgCompositionDataStatus->crpl);
    printf("BLE MSG Composotion Data Status: Features 0x%04x\n", msgCompositionDataStatus->features);
    printf("BLE MSG Composotion Data Status: Elenemt Location 0x%04x\n", msgCompositionDataStatus->location);
    printf("BLE MSG Composotion Data Status: Model count [%d]\n", msgCompositionDataStatus->model_count);
    printf("BLE MSG Composotion Data Status: Element Index 0x%02x\n", msgCompositionDataStatus->index);

    for (i = 0; i < msgCompositionDataStatus->model_count; i++)
    {
        printf("BLE MSG Composotion Data Status: Model ID [%d] 0x%04x\n", i + 1, msgCompositionDataStatus->modelID[i]);
    }
}

void print_MsgAppkeyStatus(config_msg_appkey_status_t *msgAppkeyStatus)
{
    printf("BLE MSG AppKey Status: Status 0x%02x\n", msgAppkeyStatus->status);
    printf("BLE MSG AppKey Status: Key Indexes 1 LSB 0x%02x\n", msgAppkeyStatus->key_indexes.key_id_1_lsb);
    printf("BLE MSG AppKey Status: Key Indexes 1 MSB 0x%01x\n", msgAppkeyStatus->key_indexes.key_id_1_msb);
    printf("BLE MSG AppKey Status: Key Indexes 2 LSB 0x%01x\n", msgAppkeyStatus->key_indexes.key_id_2_lsb);
    printf("BLE MSG AppKey Status: Key Indexes 2 MSB 0x%02x\n", msgAppkeyStatus->key_indexes.key_id_2_msb);
}

void print_MsgAppStatus(config_msg_app_status_t *msgAppStatus)
{
    printf("BLE MSG App Status: Status 0x%02x\n", msgAppStatus->status);
    printf("BLE MSG App Status: Element Address 0x%04x\n", msgAppStatus->element_address);
    printf("BLE MSG App Status: Appkey Index 0x%04x\n", msgAppStatus->appkey_index);
    printf("BLE MSG App Status: Model ID 0x%04x\n", msgAppStatus->model_id.sig.model_id);
}

void print_MsgPublicationStatus(config_msg_publication_status_t *msgPublicationStatus)
{
    printf("BLE MSG Publication Status: Status 0x%02x\n", msgPublicationStatus->status);
    printf("BLE MSG Publication Status: Element Address 0x%04x\n", msgPublicationStatus->element_address);
    printf("BLE MSG Publication Status: Publish Address 0x%04x\n", msgPublicationStatus->publish_address);
    printf("BLE MSG Publication Status: Appkey Index 0x%04x\n", msgPublicationStatus->state.appkey_index);
    printf("BLE MSG Publication Status: Credential Flag 0x%02x\n", msgPublicationStatus->state.credential_flag);
    printf("BLE MSG Publication Status: Rfu 0x%02x\n", msgPublicationStatus->state.rfu);
    printf("BLE MSG Publication Status: Pulish TTL 0x%02x\n", msgPublicationStatus->state.publish_ttl);
    printf("BLE MSG Publication Status: Pulish Period 0x%02x\n", msgPublicationStatus->state.publish_period);
    printf("BLE MSG Publication Status: Retranmit count 0x%02x\n", msgPublicationStatus->state.retransmit_count);
    printf("BLE MSG Publication Status: Retranmit Interval 0x%02x\n", msgPublicationStatus->state.retransmit_interval);
    printf("BLE MSG Publication Status: Model ID 0x%04x\n", msgPublicationStatus->state.model_id.sig.model_id);
}
