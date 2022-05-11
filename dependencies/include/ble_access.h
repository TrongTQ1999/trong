/* Copyright (c) 2010 - 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ACCESS_H__
#define ACCESS_H__

#include <stdint.h>
// #include "device_state_manager.h"
// #include "nrf_mesh.h"

/**
 * @defgroup ACCESS Access layer API
 * @ingroup MESH_API_GROUP_ACCESS
 * The access layer API is the main API for Mesh Models.
 *
 * The access layer API provides a way for models to register into the device database,
 * and send and receive messages on the mesh. See @ref md_doc_user_guide_modules_models_creating for a
 * walkthrough of the usage of the access layer API.
 *
 * @{
 * @defgroup ACCESS_MSCS Access layer API MSCs
 * @brief Access layer sequence diagrams
 *
 * @{
 * @mscfile model.msc "Basic access layer usage"
 * @mscfile message_rx.msc "Receiving an access layer message"
 * @mscfile periodic.msc "Periodic publishing"
 * @}
 */

/**
 * @defgroup ACCESS_DEFINES Defines
 * Access layer defines.
 * @{
 */

/** Invalid access model handle value. */
#define ACCESS_HANDLE_INVALID (0xFFFF)
/** Company ID value for Bluetooth SIG opcodes or models. */
#define ACCESS_COMPANY_ID_NONE (0xFFFF)
/** Company ID value for Nordic Semiconductor. */
#define ACCESS_COMPANY_ID_NORDIC (0x0059)

/** Invalid element index. */
#define ACCESS_ELEMENT_INDEX_INVALID (0xFFFF)

/**
 * Macro used to define a SIG model opcode.
 * @param[in] opcode Opcode of the SIG model.
 * @return Expands to an initializer for an @ref access_opcode_t struct.
 * @see access_opcode_t, access_message_tx_t
 */
#define ACCESS_OPCODE_SIG(opcode)             { (opcode), ACCESS_COMPANY_ID_NONE }

/**
 * Macro used to define a vendor model opcode.
 * @param[in] opcode  Opcode of the vendor model.
 * @param[in] company Company ID for the vendor model.
 * @return Expands to an initializer for an @ref access_opcode_t struct.
 * @see access_opcode_t, access_message_tx_t
 */
#define ACCESS_OPCODE_VENDOR(opcode, company) { (opcode), (company) }

/**
 * Macro used to define a SIG model ID.
 * @param[in] id Model ID.
 * @return Expands to an initializer for an @ref access_model_id_t struct.
 * @see access_model_id_t
 */
#define ACCESS_MODEL_SIG(id)  {.company_id = ACCESS_COMPANY_ID_NONE, .model_id = (id)}

/**
 * Macro used to define a vendor model ID.
 * @param[in] id      Model ID.
 * @param[in] company Company ID for the vendor model.
 * @return Expands to an initializer for an @ref access_model_id_t struct.
 * @see access_model_id_t
 */
#define ACCESS_MODEL_VENDOR(id, company) {.company_id = (company), .model_id = (id)}

/** Value used for TTL parameters in order to set the TTL to the default value. */
#define ACCESS_TTL_USE_DEFAULT  (0xFF)

/**
 * Maximum payload length for an access layer message.
 *
 * @note Payloads greater than @ref NRF_MESH_UNSEG_PAYLOAD_SIZE_MAX will be segmented.
 */
#define ACCESS_MESSAGE_LENGTH_MAX (NRF_MESH_SEG_PAYLOAD_SIZE_MAX)

/** Max step size used for periodic publishing. */
#define ACCESS_PUBLISH_PERIOD_STEP_MAX (0x3F)

/** Publish step resolution, number of bits. */
#define ACCESS_PUBLISH_STEP_RES_BITS (2)
/** Publish step number, number of bits. */
#define ACCESS_PUBLISH_STEP_NUM_BITS (6)
/** Publish Retransmit Count, number of bits. */
#define ACCESS_PUBLISH_RETRANSMIT_COUNT_BITS (3)
/** Publish Retransmit Interval Steps, number of bits. */
#define ACCESS_PUBLISH_RETRANSMIT_INTERVAL_STEPS_BITS (5)

/** Value used for access_publish_period_t structs when publishing is disabled. */
#define ACCESS_PUBLISH_PERIOD_NONE   { ACCESS_PUBLISH_RESOLUTION_100MS, 0 }

/** @} */

/**
 * @defgroup ACCESS_TYPES Types
 * Access layer type definitions.
 * @{
 */

/*lint -align_max(push) -align_max(1) */

/** Access layer model ID. */
typedef struct __attribute((packed))
{
    /** Company ID. Bluetooth SIG models shall set this to @ref ACCESS_COMPANY_ID_NONE. */
    uint16_t company_id;
    /** Model ID. */
    uint16_t model_id;
} access_model_id_t;

#endif /* ACCESS_H__ */
