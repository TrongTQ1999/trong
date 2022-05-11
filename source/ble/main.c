
#include "ble_cli.h"
#include "ble_evt_print.h"
#include "ble_serial.h"

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void *argv)
{
    char c;
    int fd;
    extern ble_unprovisioner_t unprovisioner;
    ble_cli_init();

    fd = ble_start();

    if (fd <= 0)
    {
        printf("\033[1;31mBLE start error \n[033[0m");
        exit(0);
    }

    printf("\033[1;32mBLE Start successfull\n\033[0m");

    while (1)
    {
        c = getchar();
        switch (c)
        {
        case '1':
            printf("\n\033[1;32mBLE scanStart\033[0m\n\n");
            ble_scanStart(fd);
            unprovisioner.state = BLE_UNPROVISIONER_STATE_NEW;
            printf("\n\033[1;32m SCANING.... \033[0m\n\n");
            break;

        case '2':
            printf("\n\033[1;32mBLE scanStop\033[0m\n\n");
            ble_scanStop(fd);
            break;

        case '3':
            ble_PacketSendGenericClientSetTrue(fd, &unprovisioner);
            printf("\n\033[1;35mPRESS NUMBER 3 FOR OFF LED      NUMBER 4 FOR ON LED \033[0m\n\n");
            break;

        case '4':
            ble_PacketSendGenericClientSetFalse(fd, &unprovisioner);
            printf("\n\033[1;35mPRESS NUMBER 3 FOR OFF LED      NUMBER 4 FOR ON LED \033[0m\n\n");
            break;

        case '5':
        {
            ble_PackerSendCompositionData(fd, &unprovisioner);
        }
        break;
        case 'r':
            printf("\n\033[1;32mBLE Node Reset\033[0m\n\n");
            ble_PacketSendNodeReset(fd, &unprovisioner);
            break;

        default:
            break;
        }
    }

    return 0;
}