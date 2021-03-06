/*
 * Copyright (c) 2012 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

#ifndef IDIGI_DEF_H_
#define IDIGI_DEF_H_

#include "ei_packet.h"

#define MSG_MAX_RECV_PACKET_SIZE 1600
#define MSG_MAX_SEND_PACKET_SIZE 512

#define UNUSED_VARIABLE(x)      ((void) (x))
#define UNUSED_PARAMETER(x)     UNUSED_VARIABLE(x)

#define ON_FALSE_DO_(cond, code)        do { if (!(cond)) {code;} } while (0)

#if (defined IDIGI_DEBUG)
#define ON_ASSERT_DO_(cond, code, output)   ON_FALSE_DO_((cond), {ASSERT(cond); code;})
#else
#define ON_ASSERT_DO_(cond, code, output)   ON_FALSE_DO_((cond), {code})
#endif

#define ASSERT_GOTO(cond, label)    ON_ASSERT_DO_((cond), {goto label;}, {})
#define CONFIRM(cond)               do { switch(0) {case 0: case (cond):;} } while (0)

#define EDP_MT_VERSION      2

#define DEVICE_TYPE_LENGTH  32
#define DEVICE_ID_LENGTH    16
#define VENDOR_ID_LENGTH    4
#define SERVER_URL_LENGTH   64
#define MAC_ADDR_LENGTH     6
#define IDIGI_MAX_TRANSACTIONS_LIMIT    255

/* these are limits for Tx and Rx keepalive
 * interval in seconds.
 */
#define MIN_TX_KEEPALIVE_INTERVAL_IN_SECONDS     5
#define MAX_TX_KEEPALIVE_INTERVAL_IN_SECONDS     7200
#define MIN_RX_KEEPALIVE_INTERVAL_IN_SECONDS     5
#define MAX_RX_KEEPALIVE_INTERVAL_IN_SECONDS     7200

/* Limits for wait count (number of
 * keepalive packets)
 */
#define WAIT_COUNT_MIN      2
#define WAIT_COUNT_MAX      64

#define MIN_VALUE(x,y)        (((x) < (y))? (x): (y))
#define MAX_VALUE(x,y)        (((x) > (y))? (x): (y))

#define URL_PREFIX  "en://"
#define GET_PACKET_DATA_POINTER(p, s)   (uint8_t *)(((uint8_t *)p) + (s))
#define IS_SEND_PENDING(idigi_ptr)      (idigi_ptr->network_connected && idigi_ptr->send_packet.total_length > 0)

#define idigi_callback_no_response(callback, class_id, request_id, request_data, request_length) idigi_callback((callback), (class_id), (request_id), (request_data), (request_length), NULL, NULL)
#define idigi_callback_no_request_data(callback, class_id, request_id, response_data, response_length) idigi_callback((callback), (class_id), (request_id), NULL, 0, (response_data), (response_length))

#if (defined IDIGI_TX_KEEPALIVE_IN_SECONDS)
#define GET_TX_KEEPALIVE_INTERVAL(idigi_ptr)    IDIGI_TX_KEEPALIVE_IN_SECONDS
#else
#define GET_TX_KEEPALIVE_INTERVAL(idigi_ptr)    idigi_ptr->tx_keepalive_interval
#endif

#if (defined IDIGI_RX_KEEPALIVE_IN_SECONDS)
#define GET_RX_KEEPALIVE_INTERVAL(idigi_ptr)    IDIGI_RX_KEEPALIVE_IN_SECONDS
#else
#define GET_RX_KEEPALIVE_INTERVAL(idigi_ptr)    idigi_ptr->rx_keepalive_interval
#endif

#if (defined IDIGI_WAIT_COUNT)
#define GET_WAIT_COUNT(idigi_ptr)    IDIGI_WAIT_COUNT
#else
#define GET_WAIT_COUNT(idigi_ptr)    idigi_ptr->wait_count
#endif

#define MAX_RECEIVE_TIMEOUT_IN_SECONDS  1
#define MIN_RECEIVE_TIMEOUT_IN_SECONDS  0

#if (IDIGI_VERSION < IDIGI_VERSION_1200)
#define asizeof(array)      (sizeof array/sizeof array[0])
#endif
/* IRL EDP States */
typedef enum {
    edp_init_layer,
    edp_communication_layer,
    edp_initialization_layer,
    edp_security_layer,
    edp_discovery_layer,
    edp_facility_layer
} idigi_edp_state_t;


typedef enum {
    idigi_device_started,
    idigi_device_stop,
    idigi_device_terminate
} idigi_active_state_t;

typedef enum {
    idigi_false,
    idigi_true,
    idigi_bool_integer_width = INT_MAX
} idigi_bool_t;
#define idigi_bool(cond)    ((cond) ? idigi_true : idigi_false)

typedef enum {
    facility_callback_delete,
    facility_callback_cleanup
} idigi_supported_facility_cb_index_t;


#define add_list_node(head, tail, node) \
    do { \
        ASSERT(node != NULL); \
        if (*head != NULL) \
        {\
            *head->prev = node;\
        }\
        node->next = *head;\
        node->prev = NULL;\
        *head = node;\
        if ((tail != NULL) && (*tail == NULL)) \
        { \
            *tail = node; \
        } \
   } while (0)

#define remove_list_node(head, tail, node) \
    do { \
        ASSERT(node != NULL); \
        if (node->next != NULL) \
        {\
            node->next->prev = node->prev;\
        }\
        if (node->prev != NULL) \
        {\
            node->prev->next = node->next;\
        }\
        if (node == *head)\
        {\
            *head = node->next;\
        }\
        if ((tail != NULL) && (node == *tail))\
        {\
            *tail = node->prev;\
        }\
    } while (0)

#define add_node(head, node) \
    do { \
        ASSERT(node != NULL); \
        if (*head != NULL) \
        {\
            *head->prev = node;\
        }\
        node->next = *head;\
        node->prev = NULL;\
        *head = node;\
    } while (0)

#define remove_node(head, node) \
    do { \
        ASSERT(node != NULL); \
        if (node->next != NULL) \
        {\
            node->next->prev = node->prev;\
        }\
        if (node->prev != NULL) \
        {\
            node->prev->next = node->next;\
        }\
        if (node == *head)\
        {\
            *head = node->next;\
        }\
    } while (0)

struct idigi_data;
struct idigi_facility;

typedef idigi_callback_status_t (* send_complete_cb_t)(struct idigi_data * const idigi_ptr, uint8_t const * const packet, idigi_status_t const status, void * const user_data);

typedef struct idigi_facility {
    unsigned int facility_index;
    size_t size;
    uint8_t * packet;
    void * facility_data;
    struct idigi_facility * next;
    struct idigi_facility * prev;
    uint16_t facility_num;
} idigi_facility_t;

typedef struct idigi_buffer {
    /* this buffer must be FIRST field in the structure
     * since this is used between network interface
     * and all facilities and facilities release the buffer
     * by returning the pointer of the buffer.
     */
    uint8_t buffer[MSG_MAX_RECV_PACKET_SIZE];
    struct idigi_buffer * next;
    idigi_bool_t    in_use;
} idigi_buffer_t;

typedef struct idigi_data {
    uint8_t * device_id;
#if !(defined IDIGI_VENDOR_ID)
    uint8_t * vendor_id;
#endif

#if !(defined IDIGI_DEVICE_TYPE)
    char * device_type;
    size_t device_type_length;
#endif

    unsigned long last_rx_keepalive_sent_time;
    unsigned long last_tx_keepalive_received_time;

    idigi_facility_t * active_facility;
    idigi_facility_t * facility_list;

    idigi_network_handle_t * network_handle;

    idigi_callback_t callback;

    idigi_active_state_t active_state;
    idigi_edp_state_t edp_state;
    idigi_status_t error_code;

    unsigned int layer_state;
    unsigned int request_id;

    idigi_bool_t network_connected;
    idigi_bool_t network_busy;
    idigi_bool_t edp_connected;

    char server_url[SERVER_URL_LENGTH];
    size_t server_url_length;
    uint8_t rx_keepalive_packet[PACKET_EDP_HEADER_SIZE];
    struct {
        struct {
            uint8_t buffer[MSG_MAX_SEND_PACKET_SIZE];
            idigi_bool_t in_use;
        } packet_buffer;
        uint8_t * ptr;
        size_t bytes_sent;
        size_t total_length;
        send_complete_cb_t complete_cb;
        void * user_data;
    } send_packet;

    struct {
        idigi_buffer_t * free_packet_buffer;
        idigi_buffer_t packet_buffer;
        uint8_t * data_packet;
        uint8_t * ptr;
        int index;
        unsigned int timeout;
        uint16_t  packet_type;
        uint16_t  packet_length;
        size_t bytes_received;
        size_t total_length;

    } receive_packet;

    uint16_t facilities;

#if !(defined IDIGI_TX_KEEPALIVE_IN_SECONDS)
    uint16_t tx_keepalive_interval;
#endif

#if !(defined IDIGI_RX_KEEPALIVE_IN_SECONDS)
    uint16_t rx_keepalive_interval;
#endif

#if !(defined IDIGI_WAIT_COUNT)
    uint16_t wait_count;
#endif

#if (IDIGI_VERSION >= IDIGI_VERSION_1200) && (!defined IDIGI_DEVICE_ID_METHOD)
    idigi_device_id_method_t device_id_method;
#endif

} idigi_data_t;

#endif /* IDIGI_DEF_H_ */
