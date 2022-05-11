
#ifndef BLE_PACKET_H
#define BLE_PACKET_H

#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    uint8_t netkey_index[2];
    uint8_t appkey_index[2];
    uint8_t *p_appkey;

} ble_config_client_appkey_add_t;

typedef struct
{
    uint8_t element_address[2];
    uint8_t appkey_index[2];
    uint8_t model_id[2];

} ble_config_client_model_app_bind_t;

#endif /* BLE_PACKET_H */