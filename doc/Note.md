# Note ble nordic mesh 5.0.0
<style>H1{color:Black;}</style>
<style>H2{color:Green;}</style>
<style>H3{color:Orange;}</style>
<style>H4{color:yellow;}</style>


## add device unprovisioner

### Example:

The first,
Innitialize for provisioning process:

    1. Provisioning Keypair Set using ecdh algorithm make key
    
        - write: 61 69 6e 1b 18 4c 05 67 95 d5 bc d8 31 fa 10 de 24 84 b5 e6 b5 88 51 32 0d 61 b1 ba b0 15 fd 6b 4e cd a6 85 f4 0f db a7 7e e1 3d 6b 09 5b 58 34 ad 73 cf f0 8b c4 59 bb df 9e 55 07 ff 6c dc fd 4c 9d c3 36 ab 7f 2d 72 82 2f b2 7f e2 c2 05 5d 45 90 60 d8 0b 02 1a 50 31 d2 a5 41 96 e8 f2 c6 a8 fe
        - write: fd 4c 9d c3 36 ab 7f 2d 72 82 2f b2 7f e2 c2 05 5d 45 90 60 d8 0b 02 1a 50 31 d2 a5 41 96 e8 f2 c6 a8 fe
        - receive: 03 84 69 00

    2. Add Addr Local unicast set
        - write: 05 9f 01 00 01 00
        -receive: 03 84 9f 00

    3. Add Subnet
        - write: 13 92 00 00 18 ee d9 c2 a5 6a dd 85 04 9f fc 3c 59 ad 0e 12
        -receive: 05 84 92 00 00 00

    4. Add Appkey (2 value)
        - write: 15 97 00 00 00 00 4f 68 ad 85 d9 f4 8a c8 58 9d f6 65 b6 b4 9b 8a
        - write: 15 97 01 00 00 00 2a a2 a6 de d5 a0 79 8c ea b5 78 7c a3 ae 39 fc   
        - receive: 05 84 97 00 00 00
        - receive: 05 84 97 00 01 00

The next, provisioning Process:
1. Scan 0
    - write: 01 61
    - receive: 03 84 61 00
    - receive: 1a c0 6e 23 f1 f9 05 b6 49 d1 ae 6a d3 0d 76 58 9b 22 db 00 01 ee 6a d3 0d 76 d8

2. Stop
    - write: 01 62
    - receive: 03 84 62 00

3. Provision 1
    - write: 2d 63 00 6e 23 f1 f9 05 b6 49 d1 ae 6a d3 0d 76 58 9b 22 18 ee d9 c2 a5 6a dd 85 04 9f fc 3c 59 ad 0e 12 00 00 00 00 00 00 1a 00 00 00 00
    - reveive: 04 84 63 00 00
    - receive: 02 c1 00
    - receive: 0b c3 00 01 00 01 00 00 00 00 00 00

4. OOB Use
    - write: 05 66 00 00 00 00
    - receive: 04 84 66 00 00
    - receive:  62 c7 00 24 2b 40 ff fb 2e 10 b2 76 9d c2 82 11 c4 cf e8 69 f5 dd e1 f4 a0 c6 bb 7d dc 6b 47 47 b9 15 5f 6c 81 28 e4 b5 fa 0a 7f 48 34 93 ea 71 e4 41 d2 a0 27 a7 43 0c f4 ca bb 5a 53 af 7d 15 4e 0c cd 6e 1b 18 4c 05 67 95 d5 bc d8 31 fa 10 de 24 84 b5 e6 b5 88 51 32 0d 61 b1 ba b0 15 fd 6b 4e cd

5. ECDH Secret (using ecdh algorithm for write command)
    - write: 22 68 00 32 4f 42 5f c8 ea 68 d9 cd a6 49 67 bd 74 a8 7e 3a 5e 71 69 58 19 5b ee 69 26 a2 a1 a3 d0 ff 84
    - receive: 04 84 68 00 00
    - receive: 2c c5 00 00 00 00 00 00 00 1a 00 00 00 86 f5 be e0 73 0e 4d b6 5c 81 14 d1 23 9b 63 cb 18 ee d9 c2 a5 6a dd 85 04 9f fc 3c 59 ad 0e 12

6. Auth Data

- Add Device Key to device:

7. Add DevKey (Add devkey to device)
    - write: 15 9c 1a 00 00 00 86 f5 be e0 73 0e 4d b6 5c 81 14 d1 23 9b 63 cb
    - receive: 05 84 9c 00 08 00

  - Add Addr Publication (Add the public address to provisioner for config) 
    - write: 03 a4 1a 00
    - receive: 05 84 a4 00 00 00
    - receive: 03 c2 00 00

9. Packet Send composition data set
    - write: 0e ab 08 00 01 00 00 00 08 00 00 00 80 08 00
    - receive: 07 84 ab 00 01 00 00 00
    - receive: 05 d2 01 00 00 00
    - receive: 30 d0 1a 00 01 00 08 00 00 00 04 01 ee 6a d3 0d 76 d8 db 1c 00 02 00 59 00 00 00 00 00 28 00 07 00 00 00 06 00 00 00 02 00 00 10 04 10 03 12 04 12

10. Packet Send Appkey Add
    - write: 1f ab 08 00 01 00 00 00 08 00 00 00 00 00 00 00 4f 68 ad 85 d9 f4 8a c8 58 9d f6 65 b6 b4 9b 8a
    - receive: 07 84 ab 00 02 00 00 00
    - receive: 05 d2 02 00 00 00
    - receive: 1a d0 1a 00 01 00 08 00 00 00 04 01 ee 6a d3 0d 76 d8 da 06 00 80 03 00 00 00 00

11. BLE Step 11: Packet Send Model Publication Set
    - BLE write: 17 ab 08 00 01 00 00 00 08 00 00 00 03 1a 00 01 00 00 00 01 00 00 00 10
    - BLE receive: 07 84 ab 00 04 00 00 00
    - BLE receive: 05 d2 04 00 00 00
    -  

12. Packet Send Model app bind
    - write: 13 ab 08 00 01 00 00 00 08 00 00 00 80 3d 1a 00 00 00 00 10
    - receive: 07 84 ab 00 05 00 00 00
    - receive: 05 d2 05 00 00 00
    - receive: 1d d0 1a 00 01 00 08 00 00 00 04 01 ee 6a d3 0d 76 d8 db 09 00 80 3e 00 1a 00 00 00 00 10

13. Packet Send GenericOnOffClient set True
    - write: 0f ab 00 00 01 00 00 00 08 00 00 00 82 02 01 01
    - receive: 07 84 ab 00 06 00 00 00
    - receive: 05 d2 06 00 00 00
    - receive: 17 d0 1a 00 01 00 00 00 00 00 04 01 ee 6a d3 0d 76 d8 da 03 00 82 04 01

14. Packet Send GenericOnOffClient set False
    - write: 0f ab 00 00 01 00 00 00 08 00 00 00 82 02 00 02
    - receive: 07 84 ab 00 07 00 00 00
    - receive: 05 d2 07 00 00 00
    - receive: 17 d0 1a 00 01 00 00 00 00 00 04 01 ee 6a d3 0d 76 d8 db 03 00 82 04 00

### NOTE: 
    Data send and receive  

### Init BLE mesh and start.

- reset radio : // why?
- set network key: // why?
- 

### add device:

### Proccess message command response:

-   message-opcode: 0x84
    - if message-response-opcode: 0x9c
        - if message-response-status: 0x00 (SUCCESS)
            - if unprovisioning.state: True
                - Process.

### Proccess mesh message receive unicast:
- print message received unicast
- data - opcode: 0x02 ( composition data status)
    - print composition data status
    - if unprovision state = BLE_UNPROVISIONER_STATE_PS_COMPOSDATA
        - add packet send appkey add
        - change unprovision state = BLE_UNPROVISIONER_STATE_PS_APPKEY_ADD
- another next step, perform with above flow