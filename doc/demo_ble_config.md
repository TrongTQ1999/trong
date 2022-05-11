<style>H1{color:Red;}</style>
<style>H2{color:Green;}</style>
<style>H3{color:Orange;}</style>
<style>H4{color:yellow;}</style>

# Using serial interface for provisioning and configuration

* This tutorial describes how use the serial interfaces to provision and configration a mesh network
* In this tutorial, a mesh node running serial example is used as a provisioner and a light switch server example is used as an unprovisoned device.

## Starting the terminal and running source code

Complete the following step:

### 1. To start the termial, run the following command
    $ ./BleGW0
        * Bluetooth Mesh Addr Local Unicast Set
            BLE write: 05 9f 01 00 01 00

            BLE receive: 03 84 9f 00 
            BLE new Command Respone: Opcode 0x9f
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x00 00 00 00
        * Bluetooth Mesh Subnet Add
            BLE write: 13 92 00 00 18 ee d9 c2 a5 6a dd 85 04 9f fc 3c 59 ad 0e 12

            BLE receive: 05 84 92 00 00 00
            BLE new Command Respone: Opcode 0x92
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x00 00 00 00
        * Bluetooth Mesh Appkey Add (2 value)
            BLE write: 15 97 00 00 00 00 4f 68 ad 85 d9 f4 8a c8 58 9d f6 65 b6 b4 9b 8a
            BLE write: 15 97 01 00 00 00 2a a2 a6 de d5 a0 79 8c ea b5 78 7c a3 ae 39 fc

            BLE receive: 05 84 97 00 00 00
            BLE new Command Respone: Opcode 0x97
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x00 00 00 00

            BLE receive: 05 84 97 00 01 00
            BLE new Command Respone: Opcode 0x97
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x01 00 00 00

### 2. Press number 1 for provision a device
    $ 3
        * Provisioning Scan Start
            BLE write: 01 61

            BLE receive: 03 84 61 00
            BLE new Command Respone: Opcode 0x61
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x01 00 00 00

            BLE receive: 1a c0 78 82 36 51 a2 d2 46 04 8c 46 63 e5 8d a4 db cd ea 00 01 8c 46 63 e5 8d e4
            BLE new Prov Unprovisioned Received: UUID 0x78823651a2d246048c4663e58da4dbcd
            BLE new Prov Unprovisioned Received: rssi [234]
            BLE new Prov Unprovisioned Received: Gatt Support [00]
            BLE new Prov Unprovisioned Received: Adv Addr Type [01]
            BLE new Prov Unprovisioned Received: Adv Addr 0x8c4663e58de4    

        * Provisioning Scan Stop
        * Provisioning Provision
            BLE write: 2d 63 00 78 82 36 51 a2 d2 46 04 8c 46 63 e5 8d a4 db cd 18 ee d9 c2 a5 6a dd 85 04 9f fc 3c 59 ad 0e 12 00 00 00 00 00 00 1b 00 00 00 00

            BLE receive: 04 84 63 00 00
            BLE new Command Respone: Opcode 0x63
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x00 51 a2 d2

            BLE receive: 02 c1 00 
            BLE new Prov Link Established: Context ID 0x00

            BLE receive: 0b c3 00 01 00 01 00 00 00 00 00 00
            BLE new Prov Caps Received: Context ID 0x00
            BLE new Prov Caps Received: Num Element [1]
            BLE new Prov Caps Received: Public Key Type 0x00
            BLE new Prov Caps Received: Static OOB Types 0x01
            BLE new Prov Caps Received: Output OOB Size 0x00
            BLE new Prov Caps Received: Output OOB Actions 0x0000
            BLE new Prov Caps Received: Input OOB Size 0x00
            BLE new Prov Caps Received: Input OOB Actions 0x0000
        * Provisioning OOB Use
            BLE write: 05 66 00 01 00 00 

            BLE receive: 04 84 66 00 00
            BLE new Command Respone: Opcode 0x66
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x00 01 00 00

            BLE receive: 62 c7 00 3a ee 49 a5 76 56 18 06 f8 6a 0f 16 74 05 8c f6 c4 77 c3 ca 9c 0b 75 5e 26 5d 6e 01 d4 df fd 79 d8 37 ee 79 99 5a 0f 91 67 e4 18 32 0f cc 07 fe b7 3f cb 97 6a 27 1d 83 c3 f7 cd fa 6e 39 4e 75 10 d2 94 0b 03 9a 00 e3 3d 21 05 42 3d ec 82 28 51 c0 56 53 a9 20 52 2b 6c 97 60 a7 e4 dc 88 3e
            BLE new Prov ECDH Request: Context ID: [0x00]
            BLE new Prov ECDH Request: Peer Public : 3a ee 49 a5 76 56 18 06 f8 6a 0f 16 74 05 8c f6 c4 77 c3 ca 9c 0b 75 5e 26 5d 6e 01 d4 df fd 79 d8 37 ee 79 99 5a 0f 91 67 e4 18 32 0f cc 07 fe b7 3f cb 97 6a 27 1d 83 c3 f7 cd fa 6e 39 4e 75
            BLE new Prov Unprovisioned Received: Node private : 10 d2 94 0b 03 9a 00 e3 3d 21 05 42 3d ec 82 28 51 c0 56 53 a9 20 52 2b 6c 97 60 a7 e4 dc 88 3e
        * Provisioning ECDH Secret
            BLE write: 22 68 00 85 ab 81 06 5a 72 3d 8b f3 b8 cf 41 8a 37 b1 4b 58 52 e7 99 4c 02 f5 8c 09 eb 33 35 6f 58 81 29

            BLE receive: 05 c6 00 01 00 10 04 84 68 00 00
            BLE new Prov Auth Request: Context ID 0x00
            BLE new Prov Auth Request: Method [01]
            BLE new Prov Auth Request: Action 0x00
            BLE new Prov Auth Request: Size 0x10
            LE new Command Respone: Opcode 0x68
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x00 10 1a f4
        * Provisioning Auth Data
            BLE write: 12 67 00 6e 6f 72 64 69 63 5f 65 78 61 6d 70 6c 65 5f 31

            BLE receive: 04 84 67 00 00

            BLE receive: 2c c5 00 00 00 00 00 00 00 1b 00 00 00 11 9a 3b ad 12 86 5d 96 5c 6f a9 41 af e2 aa 7c 18 ee d9 c2 a5 6a dd 85 04 9f fc 3c 59 ad 0e 12
            BLE new Prov Complete: Context ID 0x00
            BLE new Prov Complete: Iv Index 0x00000000
            BLE new Prov Complete: Net Key Index 0x0000
            BLE new Prov Complete: Address 0x1b00   
            BLE new Prov Complete: Iv Update Flag 0x00
            BLE new Prov Complete: Key Refresh Flag [00]
            BLE new Prov Complete: Device Key 0x119a3bad12865d965c6fa941afe2aa7c
            BLE new Prov Complete: Net Key 0x18eed9c2a56add85049ffc3c59ad0e12
        * Bluetooth Mesh Devkey Add
            BLE write: 15 9c 1b 00 00 00 11 9a 3b ad 12 86 5d 96 5c 6f a9 41 af e2 aa 7c

            BLE receive: 05 84 9c 00 08 00 05 84 a4 00 00 00
            BLE new Command Respone: Opcode 0x9c
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x08 00 00 00
        * Bluetooth Mesh Addr Publication Add
            BLE write: 03 a4 1b 00

            BLE new Command Respone: Opcode 0xa4
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x00 00 00 00

            BLE receive: 03 c2 00 00
            BLE new Prov Link Closed: Context ID 0x00
            BLE new Prov Link Closed: Close Reason [00]
        * Bluetooth Mesh Packet Send Composition Data Set
            BLE write: 0e ab 08 00 01 00 00 00 08 00 00 00 80 08 00

            BLE receive: 07 84 ab 00 01 00 00 00
            BLE new Command Respone: Opcode 0xab
            BLE new Command Respone: status 0x00
            BLE new Command Respone: data 0x01 00 00 00

            BLE receive: 05 d2 01 00 00 00
            BLE new Mesh TX Complete: Token 0x00000001
            
### 3. Press number 4 for Add Appkey to Device and Bind Model App
    $ 4
        * Bluetooth Mesh Packet Send Appkey Add
        * Bluetooth Mesh Packet Send Model App Bind
### 4. Press numbber 5 for Set Model Publication 
    $ 5
        * Enter the desired unicast address
        * Bluetooth Mesh Packet Send Model Publication Set
### 5. Press number 6 for Control Generic OnOff Client Set
    $ 6
        * Enter "ON" for TURN ON the LED
        * Enter "OFF" for TURN OFF the LED

