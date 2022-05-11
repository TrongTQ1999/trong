#ifndef BLE_EVT_PRINT_H__
#define BLE_EVT_PRINT_H__

#include "ble_cli.h"
#include "ble_config_messages.h"


#include <stdio.h>

/* Function Print Data */
void print_ProvUnprovisioned(ble_ProvUnprovisionedRecv_t *provUnprovisioned);
void print_ProvLinkEsta(ble_ProvLinkEstablish_t *provLinkEsta);
void print_ProvCapsRecv(ble_ProvCapsRecv_t *provCapsRecv);
void print_ProvECDHReq(ble_ProvECDHReq_t *provECDHReq);
void print_ProvAuthReq(ble_ProvAuthReq_t *provAuthReq);
void print_ProvComplete(ble_ProvComplete_t *provComplete);
void print_LinkClose(ble_ProvLinkCl_t *provLinkClo);
void print_CmdRsp(ble_CmdRsp_t *cmdRsp, int size_data);
void print_MeshMessageRec(serial_evt_mesh_message_received_t *meshMessageRec, int size_data);

void print_deviceStarted(serial_evt_device_started_t *deviceStarted);
void print_MeshIvEntrySetNoti(serial_evt_mesh_iv_entry_set_notification_t *ivEntruSetNoti);
void print_MeshSeqnumEntrySetNoti(serial_evt_mesh_seqnum_entry_set_notification_t *seqnumSetNoti);

void print_MeshTXComplete(serial_evt_mesh_tx_complete_t *txComplete);
void print_ProvFailed(serial_evt_prov_failed_t *provFailed);


void print_MsgCompositionDataStatus(config_msg_composition_data_status_t *msgCompositionDataStatus);
void print_MsgAppkeyStatus(config_msg_appkey_status_t *msgAppkeyStatus);
void print_MsgAppStatus(config_msg_app_status_t *msgAppStatus);
void print_MsgPublicationStatus(config_msg_publication_status_t *msgPublicationStatus);


#endif /* BLE_EVT_PRINT_H__ */