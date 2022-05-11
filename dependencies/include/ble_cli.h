#ifndef __BLE_CLI__
#define __BLE_CLI__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "ble_evt.h"
#include "ble_cmd.h"
#include "serial_status.h"

typedef enum
{
    BLE_UNPROVISIONER_STATE_NEW,
    BLE_UNPROVISIONER_STATE_PROVISION,
    BLE_UNPROVISIONER_STATE_OOBUSE,
    BLE_UNPROVISIONER_STATE_ECDH,
    BLE_UNPROVISIONER_STATE_AUTH,
    BLE_UNPROVISIONER_STATE_DEVKEY_ADDRPUBADD,
    BLE_UNPROVISIONER_STATE_PS_COMPOSDATA,
    BLE_UNPROVISIONER_STATE_PS_APPKEY_ADD,
    BLE_UNPROVISIONER_STATE_PS_MODEL_APP_BIND,
    BLE_UNPROVISIONER_STATE_PS_MODEL_PUBLICATION_SET,
} BLE_UNPROVISIONER_STATE_E;

typedef struct
{
    uint8_t state; // BLE_UNPROVISIONER_STATE_E
    uint16_t dst_addr_handle;
    uint16_t devkey_handle;
    uint16_t unicast;
    uint16_t subnet_handle;
    uint16_t element_address;
} ble_unprovisioner_t;

// ble_unprovisioner_t unprovisioner;

void ble_cli_init();

// thread for read data in buffer
void *ble_receive_handle(void *arg);

void ble_processCommand(int fd);

int ble_start();

#endif // __BLE_CLI__
