/*
 *  Copyright (c) 1996-2011 Digi International Inc., All Rights Reserved
 *
 *  This software contains proprietary and confidential information of Digi
 *  International Inc.  By accepting transfer of this copy, Recipient agrees
 *  to retain this software in confidence, to prevent disclosure to others,
 *  and to make no use of this software other than that for which it was
 *  delivered.  This is an unpublished copyrighted work of Digi International
 *  Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
 *  prohibited.
 *
 *  Restricted Rights Legend
 *
 *  Use, duplication, or disclosure by the Government is subject to
 *  restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
 *  Technical Data and Computer Software clause at DFARS 252.227-7031 or
 *  subparagraphs (c)(1) and (2) of the Commercial Computer Software -
 *  Restricted Rights at 48 CFR 52.227-19, as applicable.
 *
 *  Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 *
 * =======================================================================
 *
 */
#include "ei_discover.h"
#include "ei_security.h"
#include "ei_msg.h"

/*
 * MT version 2 message type defines.
 * Refer to EDP specification rev. 14.2 for a description of MT version 2.
 */
#define E_MSG_MT2_TYPE_VERSION                          0x0004 /* C -> S */
#define E_MSG_MT2_TYPE_LEGACY_EDP_VERSION     0x0004 /* C -> S */
#define E_MSG_MT2_TYPE_LEGACY_EDP_VER_RESP   0x0001 /* C <- S */
#define E_MSG_MT2_TYPE_VERSION_OK                    0x0010 /* C <- S */
#define E_MSG_MT2_TYPE_VERSION_BAD                  0x0011 /* C <- S */
#define E_MSG_MT2_TYPE_SERVER_OVERLOAD         0x0012 /* C <- S */
#define E_MSG_MT2_TYPE_KA_RX_INTERVAL              0x0020 /* C -> S */
#define E_MSG_MT2_TYPE_KA_TX_INTERVAL              0x0021 /* C -> S */
#define E_MSG_MT2_TYPE_KA_WAIT                          0x0022 /* C -> S */
#define E_MSG_MT2_TYPE_KA_KEEPALIVE                 0x0030 /* bi-directional */
#define E_MSG_MT2_TYPE_PAYLOAD                         0x0040 /* bi-directional */

#define ONE_SECOND  1


static bool valid_timing_limit(idigi_data_t * const idigi_ptr, uint32_t const start, uint32_t const limit)
{
    uint32_t elapsed;
    bool rc = false;

    if (get_system_time(idigi_ptr, &elapsed) == idigi_callback_continue)
    {
        elapsed -= start;
        rc = (elapsed < limit);
    }

    return rc;
}

static void set_idigi_state(idigi_data_t * const idigi_ptr, int const state)
{
    /* set to new edp state */
    idigi_ptr->edp_state = state;
    idigi_ptr->layer_state = 0;
    idigi_ptr->request_id = 0;
}


static idigi_callback_status_t enable_send_packet(idigi_data_t * const idigi_ptr, uint8_t * const packet, uint16_t const length, uint16_t const type, send_complete_cb_t send_complete_cb, void * const user_data)
{
    idigi_callback_status_t status = idigi_callback_continue;
    uint8_t * const edp_header = packet;

    /* Setup data to be sent. send_packet_process() will actually
     * send out the data.
     */

    if (idigi_ptr->send_packet.total_length > 0)
    {
        DEBUG_PRINTF("enable_send_packet: unable to trigger another send since previous data is still pending\n");
        status = idigi_callback_busy;
        goto done;
    }

    /*
     * MTv2 (and later)...
     * Packet format for MT version:
     *   | type | length | data |
     *
     * It has a 2-octet type field and the 2-octet length field and followed by data.
     *
     */
    message_store_be16(edp_header, type, type);
    message_store_be16(edp_header, length, length);

    /* total bytes to be sent to server (packet data length + the edp header length) */
    idigi_ptr->send_packet.total_length = length + PACKET_EDP_HEADER_SIZE;
    idigi_ptr->send_packet.ptr = (uint8_t *)packet;

    /* clear the actual number of bytes to be sent */
    idigi_ptr->send_packet.length = 0;
    idigi_ptr->send_packet.complete_cb = send_complete_cb;
    idigi_ptr->send_packet.user_data = user_data;
done:
    return status;
}

static idigi_callback_status_t enable_facility_packet(idigi_data_t * const idigi_ptr, uint8_t * const edp_header,
                                                                                       uint16_t const length, uint16_t const facility,
                                                                                       send_complete_cb_t send_complete_cb, void * const user_data)
{
    uint8_t * const edp_protocol = edp_header + PACKET_EDP_HEADER_SIZE;


    /* this function is to set up a facility packet to be sent.
     * Setup security coding, discovery payload & facility.
     *
     * facility packet:
     *    ----------------------------------------------------------------------------------------------------------------------------------
     *   |             1 - 4             |               5                 |              6             |  7 - 8   |       9...       |
     *    -----------------------------------------------------------------------------------------------------------------------------------
     *   | MTv2 packet format | data security coding | discovery payload | facility | facility data|
     *    -----------------------------------------------------------------------------------------------------------------------------------
     *   |       EDP Header       |                           EDP  Protocol                                                 |
     *    -----------------------------------------------------------------------------------------------------------------------------------
     */
    message_store_u8(edp_protocol, sec_coding, SECURITY_PROTO_NONE);
    message_store_u8(edp_protocol, payload, DISC_OP_PAYLOAD);
    message_store_be16(edp_protocol, facility, facility);

    return enable_send_packet(idigi_ptr, edp_header,
                              (length + PACKET_EDP_PROTOCOL_SIZE),
                              E_MSG_MT2_TYPE_PAYLOAD,
                              send_complete_cb,
                              user_data);
}

static int send_buffer(idigi_data_t * const idigi_ptr, uint8_t * const buffer, size_t const length)
{
    int bytes_sent = 0;

    idigi_callback_status_t status;
    idigi_write_request_t write_data;
    uint32_t tx_ka_timeout;
    uint32_t rx_ka_timeout;
    uint32_t time_stamp;
    idigi_request_t const request_id = {idigi_network_send};

    size_t length_written, size;

    if (idigi_ptr->network_busy || idigi_ptr->network_handle == NULL)
    {
        /* don't do any network activity */
        goto done;
    }

    /* This function actually calls app's callback to sent data
     * to the server. It sets up timeout value which callback
     * must return within the timeout value.
     *
     * Get rx keepalive timeout that we need to send a keepalive packet and
     * tx keepalive timeout that we expect a keepalive packet from server.
     */
    status =  get_keepalive_timeout(idigi_ptr, &rx_ka_timeout, &tx_ka_timeout, &time_stamp);
    if (status != idigi_callback_continue)
    {
        /*  needs to return immediately for rx_keepalive. */
        if (status == idigi_callback_abort)
        {
            bytes_sent = -idigi_ptr->error_code;
        }
        goto done;
    }

    write_data.timeout = MIN_VALUE(rx_ka_timeout, tx_ka_timeout);

    write_data.buffer = buffer;
    write_data.length = length;
    write_data.network_handle = idigi_ptr->network_handle;

    size = sizeof length_written;
    status = idigi_callback(idigi_ptr->callback, idigi_class_network, request_id, &write_data, sizeof write_data, &length_written, &size);
    if (status == idigi_callback_continue && length_written > 0) 
    {
        bytes_sent = length_written;

        /* Retain the "last (RX) message send" time. */
        status = get_system_time(idigi_ptr, &idigi_ptr->rx_ka_time);
        if (status == idigi_callback_abort)
        {
            bytes_sent = -idigi_configuration_error;
        }
    }
    else if (status == idigi_callback_abort)
    {
        bytes_sent = -idigi_send_error;
    }

done:
    return bytes_sent;
}

static void release_packet_buffer(idigi_data_t * const idigi_ptr, uint8_t const * const packet, idigi_status_t const status, void * const user_data)
{
    /* this is called when it's done sending or after get_packet_buffer()
     * is called to release idigi_ptr->send_packet.packet_buffer.buffer.
     *
     */
    UNUSED_PARAMETER(status);
    ASSERT(idigi_ptr->send_packet.packet_buffer.buffer == packet);
    UNUSED_PARAMETER(packet);
    UNUSED_PARAMETER(user_data);

    idigi_ptr->send_packet.packet_buffer.in_used = false;
    idigi_ptr->send_packet.packet_buffer.facility = E_MSG_MT2_MSG_NUM;
}

static uint8_t * get_packet_buffer(idigi_data_t * const idigi_ptr, uint16_t const facility, uint8_t ** data_ptr)
{
    uint8_t * packet = NULL;
    uint8_t * ptr = NULL;

    /* Return a  pointer to caller to setup data to be sent to server.
     * Must call release_packet_buffer() to release the buffer (pass this
     * release_packet_buffer as complete_callback to enable_send_packet()
     * or enable_facility_packet().
     */

     /* make sure no send is pending */
    if ((idigi_ptr->send_packet.total_length == 0) &&
        ((!idigi_ptr->send_packet.packet_buffer.in_used) ||
         (idigi_ptr->send_packet.packet_buffer.facility == facility)))
    {
        idigi_ptr->send_packet.packet_buffer.in_used = true;
        idigi_ptr->send_packet.packet_buffer.facility = facility;

        packet = (uint8_t *)idigi_ptr->send_packet.packet_buffer.buffer;
        ptr = GET_PACKET_DATA_POINTER(packet, PACKET_EDP_HEADER_SIZE);

        if (facility != E_MSG_MT2_MSG_NUM)
        {
            /* facility packet */
            ptr += PACKET_EDP_PROTOCOL_SIZE;
        }
    }
#if defined(DEBUG)
    else
    {
        DEBUG_PRINTF("get packet buffer: send pending\n");
    }
#endif
    *data_ptr = ptr;
    return packet;
}

static idigi_callback_status_t rx_keepalive_process(idigi_data_t * const idigi_ptr)
{
   idigi_callback_status_t status = idigi_callback_continue;

    if (idigi_ptr->network_handle == NULL)
    {
        goto done;
    }

    /* Sends rx keepalive if keepalive timing is expired.
     *
     * rx_ka_time is last time we sent Rx keepalive.
     * rx_keepalive is Rx keepalive message frequency.
     */
    if (valid_timing_limit(idigi_ptr, idigi_ptr->rx_ka_time, *idigi_ptr->rx_keepalive))
    {
        /* not expired yet. no need to send rx keepalive */
        goto done;
    }

    DEBUG_PRINTF("rx_keepalive_process: time to send Rx keepalive\n");

    status = enable_send_packet(idigi_ptr, (uint8_t * const)&idigi_ptr->rx_keepalive_packet, 0, E_MSG_MT2_TYPE_KA_KEEPALIVE, NULL, NULL);

done:
    return status;
}

static void send_complete_callback(idigi_data_t * const idigi_ptr)
{
    if (idigi_ptr->send_packet.complete_cb != NULL)
    {
        idigi_ptr->send_packet.complete_cb(idigi_ptr, idigi_ptr->send_packet.ptr, idigi_ptr->error_code, idigi_ptr->send_packet.user_data);
        idigi_ptr->send_packet.complete_cb = NULL;
    }
}

static idigi_callback_status_t send_packet_process(idigi_data_t * const idigi_ptr)
{
    uint8_t * buf;
    size_t  length;
    int bytes_sent;
    idigi_callback_status_t status = idigi_callback_continue;

    /* if nothing needs to be sent, check whether we need to send rx keepalive */
    if (idigi_ptr->send_packet.total_length == 0 && idigi_ptr->edp_state >= edp_discovery_layer)
    {

        rx_keepalive_process(idigi_ptr);
    }

    if (idigi_ptr->send_packet.total_length > 0)
    {
        /* We have something to be sent */
        buf = idigi_ptr->send_packet.ptr + idigi_ptr->send_packet.length;
        length = idigi_ptr->send_packet.total_length;

        bytes_sent = send_buffer(idigi_ptr, buf, length);
        if (bytes_sent > 0)
        {
            idigi_ptr->send_packet.total_length -= bytes_sent;
            idigi_ptr->send_packet.length += bytes_sent;
        }
        else
        {
            idigi_ptr->error_code = -bytes_sent;
            status = idigi_callback_abort;
        }

        if (idigi_ptr->send_packet.total_length == 0)
        {   /* sent completed so let's call the complete callback */
            send_complete_callback(idigi_ptr);
        }
    }

    if (status == idigi_callback_continue && idigi_ptr->send_packet.total_length > 0)
    {
        status = idigi_callback_busy;
    }
    return status;

}

static uint8_t * new_receive_packet(idigi_data_t * const idigi_ptr)
{
    idigi_buffer_t * buffer_ptr;
    uint8_t * packet = NULL;

    /* return an available packet for receiving data */
    buffer_ptr = idigi_ptr->receive_packet.free_packet_buffer;
    if (buffer_ptr != NULL)
    {
        packet = buffer_ptr->buffer;
        idigi_ptr->receive_packet.free_packet_buffer = buffer_ptr->next;
    }
#if defined(DEBUG)
    if (buffer_ptr == NULL)
    {
        DEBUG_PRINTF("new_receive_packet: no buffer available for receiving message from server\n");
    }
#endif

    return packet;
}

static void release_receive_packet(idigi_data_t * const idigi_ptr, uint8_t const * const packet)
{
    idigi_buffer_t * buffer_ptr = (idigi_buffer_t *)packet;

    ASSERT(packet != NULL);

    /* release a packet that is from new_receive_packet() */
    if (packet != NULL)
    {
        buffer_ptr->next = idigi_ptr->receive_packet.free_packet_buffer;
        idigi_ptr->receive_packet.free_packet_buffer = buffer_ptr;
    }
    return;
}


static int receive_buffer(idigi_data_t * const idigi_ptr, uint8_t  * const buffer, size_t const length)
{
    int bytes_received = 0;
    idigi_request_t const request_id = {idigi_network_receive};

    /* Call callback to receive data from server */

    if (!idigi_ptr->network_busy)
    {
        idigi_read_request_t read_data;
        idigi_callback_status_t status;
        uint32_t tx_ka_timeout;
        uint32_t rx_ka_timeout;
        uint32_t time_stamp;
        size_t  length_read;
        size_t size;

        /* Get rx keepalive timeout that we need to send a keepalive packet and
         * tx keepalive timeout that we expect a keepalive packet from server.
         */
        status =  get_keepalive_timeout(idigi_ptr, &rx_ka_timeout, &tx_ka_timeout, &time_stamp);
        if (status != idigi_callback_continue)
        {
            /*  needs to return immediately for rx_keepalive. */
            if (status == idigi_callback_abort)
            {
                bytes_received = -idigi_ptr->error_code;
            }
            goto done;
        }
        if (tx_ka_timeout == 0 || rx_ka_timeout == 0)
        {
            read_data.timeout = ONE_SECOND;
        }
        else
        {
            read_data.timeout = MIN_VALUE(tx_ka_timeout, rx_ka_timeout);
        }

        read_data.network_handle = idigi_ptr->network_handle;
        read_data.buffer = buffer;
        read_data.length = length;

        size = sizeof length_read;

        status = idigi_callback(idigi_ptr->callback, idigi_class_network, request_id, &read_data, sizeof read_data, &length_read, &size);
        if (status == idigi_callback_abort)
        {
            DEBUG_PRINTF("receive_buffer: callback returns abort\n");
            bytes_received = -idigi_receive_error;
            goto done;
        }
        else if (status == idigi_callback_busy)
        {
            goto done;
        }

        if (length_read > 0)
        {
            bytes_received = (int)length_read;
            /* Retain the "last (tx keepalive) message send" time. */
            if (get_system_time(idigi_ptr, &idigi_ptr->tx_ka_time) != idigi_callback_continue)
            {
                bytes_received = -idigi_configuration_error;
            }
            goto done;
        }
    }

    /* check Tx keepalive timing */
    if (*idigi_ptr->tx_keepalive > 0)
    {
        uint8_t wait_count = * idigi_ptr->wait_count;
        uint32_t max_timeout = *idigi_ptr->tx_keepalive * wait_count;

        if (!valid_timing_limit(idigi_ptr, idigi_ptr->tx_ka_time, max_timeout))
        {
            /*
             * We haven't received a message
             * of any kind for the configured maximum interval, so we must
             * mark this connection in error and return that status.
             *
             * Note: this inactivity check applies only for MTv2 and later.
             * For MTv1, the client sends keep-alives, but the server does
             * not send them (nor must the client expect them). For MTv1,
             * the data member tx_keepalive_ms is always 0 (zero), so this
             * keep-alive failure check never triggers.
             *
             */
            bytes_received = -idigi_keepalive_error;
            notify_error_status(idigi_ptr->callback, idigi_class_config, request_id, idigi_keepalive_error);
            DEBUG_PRINTF("idigi_receive: keepalive fails\n");
        }
    }

done:
    return bytes_received;
}


static idigi_callback_status_t receive_data_status(idigi_data_t * const idigi_ptr)
{
    idigi_callback_status_t status = idigi_callback_continue;

    /* send data if we have more data to send */
    if (idigi_ptr->receive_packet.length < idigi_ptr->receive_packet.total_length)
    {
        uint8_t * buf;
        size_t  length;
        int     read_length;

        buf = idigi_ptr->receive_packet.ptr + idigi_ptr->receive_packet.length;
        length = idigi_ptr->receive_packet.total_length - idigi_ptr->receive_packet.length;

        read_length = receive_buffer(idigi_ptr, buf, length);
        if (read_length > 0)
        {
            idigi_ptr->receive_packet.length += read_length;
        }
        else if (read_length < 0)
        {
            idigi_ptr->error_code = -read_length;
            status = idigi_callback_abort;
            goto done;
        }
    }

    if (idigi_ptr->receive_packet.total_length > 0 &&
        idigi_ptr->receive_packet.length < idigi_ptr->receive_packet.total_length)
    {
        /* still more data */
        status = idigi_callback_busy;
    }
done:
    return status;
}


static idigi_callback_status_t receive_packet(idigi_data_t * const idigi_ptr, uint8_t ** packet)
{
    typedef enum {
        receive_packet_init,
        receive_packet_type,
        receive_packet_length,
        receive_packet_data,
        receive_packet_complete
    } receive_packet_index_t;

    idigi_callback_status_t status = idigi_callback_continue;

    *packet = NULL;

    if (idigi_ptr->network_handle == NULL)
    {
        goto done;
    }

    /*
     * Read the MT message type.
     *
     * For MT version 2, there are numerous message types. Some of these
     * messages require special handling, in that they may be legacy EDP
     * version response message varieties. These messages are sent by
     * servers that do not support MTv2. Since the client doesn't support
     * both MTv1 and MTv2 concurrently, an MTv2 client must terminate its
     * MT connection if it finds the server to be incompatible insofar as
     * the MT version is concerned.
     *
     * We only accept messages of the expected types from the server. Any
     * message other than an expected type is handled as an error, and an
     * error is returned to the caller. This must be done since any unknown
     * message type cannot be correctly parsed for length and discarded from
     * the input stream.
     */

    /* we have to read 3 times to get a complete packet.
     * 1. read message type
     * 2. read message length
     * 3. read actual message data
     *
     * So we use index == receive_packet_init to initialize the packet before starting reading.
     * When index == receive_packet_type, set to receive message type. After message type is received,
     * we must check valid message type.
     * 
     * When index == receive_packet_length, set to receive message length.
     * When index == receive_packet_data, set to receive message data.
     * When index == receive_packet_complete, message data is completely received and
     * reset index = receive_packet_init and exit.
     *
     */
    while (idigi_ptr->receive_packet.index <= receive_packet_complete)
    {
        if (idigi_ptr->receive_packet.index != receive_packet_init)
        {   /* continue for any pending receive */
            status = receive_data_status(idigi_ptr);
            if (status != idigi_callback_continue)
            {  /* receive pending */
                goto done;
            }
        }

        switch (idigi_ptr->receive_packet.index)
        {
        case receive_packet_init:
            /* initialize and setup packet for receive */
            idigi_ptr->receive_packet.packet_type = 0;
            idigi_ptr->receive_packet.packet_length = 0;
            idigi_ptr->receive_packet.length = 0;
            idigi_ptr->receive_packet.total_length = 0;
            idigi_ptr->receive_packet.index = 0;
            idigi_ptr->receive_packet.data_packet = new_receive_packet(idigi_ptr);
            if (idigi_ptr->receive_packet.data_packet == NULL)
            {
                goto done;
            }
            idigi_ptr->receive_packet.index++;
            break;

        case receive_packet_type:
            /* set to read the message type */
            idigi_ptr->receive_packet.ptr = (uint8_t *)&idigi_ptr->receive_packet.packet_type;
            idigi_ptr->receive_packet.length = 0;
            idigi_ptr->receive_packet.total_length = sizeof idigi_ptr->receive_packet.packet_type;
            idigi_ptr->receive_packet.index++;
            break;

        case receive_packet_length:
        {
            /* Got message type let's get to message length.
             * So make sure we support the message type.
             * Then, set to read message length. 
             */
            uint16_t type_val;

            type_val = FROM_BE16(idigi_ptr->receive_packet.packet_type);
            idigi_ptr->receive_packet.packet_type = type_val;

            switch (type_val)
            {
                /* Expected MTv2 message types... */
                case E_MSG_MT2_TYPE_LEGACY_EDP_VER_RESP:
                {
                    uint8_t * edp_header = idigi_ptr->receive_packet.data_packet;

                    DEBUG_PRINTF("receive_packet: E_MSG_MT2_TYPE_LEGACY_EDP_VER_RESP 0x%x\n", (unsigned) type_val);
                    /*
                     * Obtain the MT message length (2 bytes).
                     * Note that legacy EDP version response messages do not have a length
                     * field. There is just a single byte of data remaining in the stream
                     * for this MT message type, so we provide a dummy length value of 1 in
                     * this case. All other MT message types do have a length field, which
                     * we will validate according to message type after we read the length
                     * field bytes.
                     */
                    /* Supply a length of 1 byte. */
                    message_store_be16(edp_header, length, 1);

                    /*
                     * Read the actual message data bytes into the packet buffer.
                     */
                    idigi_ptr->receive_packet.index++; /* set to read message data */
                    idigi_ptr->receive_packet.ptr = GET_PACKET_DATA_POINTER(idigi_ptr->receive_packet.data_packet,
                                                                                                                PACKET_EDP_HEADER_SIZE);
                    idigi_ptr->receive_packet.length = 0;

                    idigi_ptr->receive_packet.total_length = 1; /* length to receive */

                    break;
                }
                case E_MSG_MT2_TYPE_VERSION_OK:
                    break;
                case E_MSG_MT2_TYPE_VERSION_BAD:
                    DEBUG_PRINTF("receive_packet: E_MSG_MT2_TYPE_VERSION_BAD 0x%x\n", (unsigned) type_val);
                    break;
                case E_MSG_MT2_TYPE_SERVER_OVERLOAD:
                    DEBUG_PRINTF("receive_packet: E_MSG_MT2_TYPE_SERVER_OVERLOAD 0x%x\n", (unsigned) type_val);
                    break;
                case E_MSG_MT2_TYPE_KA_KEEPALIVE:
                    break;
                case E_MSG_MT2_TYPE_PAYLOAD:
                    break;
                /* Unexpected/unknown MTv2 message types... */
                case E_MSG_MT2_TYPE_VERSION:
                case E_MSG_MT2_TYPE_KA_RX_INTERVAL:
                case E_MSG_MT2_TYPE_KA_TX_INTERVAL:
                case E_MSG_MT2_TYPE_KA_WAIT:
                default:
                {
                    /* tell caller we have unexpected packet message .
                     * If callback tells us to continue, we continue reading packet data.
                     */
                    idigi_request_t const request_id = {idigi_network_receive};
                    DEBUG_PRINTF("receive_packet: error type 0x%x\n", (unsigned) type_val);
                    idigi_ptr->error_code = idigi_invalid_packet;
                    notify_error_status(idigi_ptr->callback, idigi_class_network, request_id, idigi_invalid_packet);
                }
           }

            if (type_val != E_MSG_MT2_TYPE_LEGACY_EDP_VER_RESP)
            {   /* set up to read message length */
                idigi_ptr->receive_packet.ptr = (uint8_t *)&idigi_ptr->receive_packet.packet_length;
                idigi_ptr->receive_packet.length = 0;
                idigi_ptr->receive_packet.total_length = sizeof idigi_ptr->receive_packet.packet_length;
                idigi_ptr->receive_packet.index++;
            }
            break;
        }
        case receive_packet_data:
            /* got packet length so set to read message data */
            idigi_ptr->receive_packet.packet_length = FROM_BE16(idigi_ptr->receive_packet.packet_length);

            if (idigi_ptr->receive_packet.packet_type != E_MSG_MT2_TYPE_PAYLOAD)
            {
                /*
                 * For all but payload messages, the length field value should be
                 * zero, as there is no data accompanying the message. The MT
                 * messages to which this applies here are:
                 *    E_MSG_MT2_TYPE_VERSION_OK
                 *    E_MSG_MT2_TYPE_VERSION_BAD
                 *    E_MSG_MT2_TYPE_SERVER_OVERLOAD
                 *    E_MSG_MT2_TYPE_KA_KEEPALIVE
                 */
                if (idigi_ptr->receive_packet.packet_length != 0)
                {
                    idigi_request_t const request_id = {idigi_network_receive};
                    DEBUG_PRINTF("idigi_get_receive_packet: Invalid payload\n");
                    idigi_ptr->error_code = idigi_invalid_payload_packet;
                    notify_error_status(idigi_ptr->callback, idigi_class_config, request_id, idigi_invalid_payload_packet);
                }
            }
            /* set to read message data */
            idigi_ptr->receive_packet.total_length = idigi_ptr->receive_packet.packet_length;

            if (idigi_ptr->receive_packet.packet_length == 0)
            {
                /* set to complete data since no data to be read. */
                idigi_ptr->receive_packet.index = receive_packet_complete;
            }
            else 
            {
                ASSERT(idigi_ptr->receive_packet.packet_length <= (MSG_MAX_PACKET_SIZE - PACKET_EDP_HEADER_SIZE));
                /*
                 * Read the actual message data bytes into the packet buffer.
                 */
                idigi_ptr->receive_packet.ptr = GET_PACKET_DATA_POINTER(idigi_ptr->receive_packet.data_packet, PACKET_EDP_HEADER_SIZE);
                idigi_ptr->receive_packet.length = 0;
                idigi_ptr->receive_packet.index++;

            }
            break;
        case receive_packet_complete:
        {
            uint8_t * edp_header = idigi_ptr->receive_packet.data_packet;
            /* got message data. Let's set edp header */
            message_store_be16(edp_header, type, idigi_ptr->receive_packet.packet_type);
            message_store_be16(edp_header, length, idigi_ptr->receive_packet.packet_length);
            idigi_ptr->receive_packet.index = receive_packet_init;
            *packet = idigi_ptr->receive_packet.data_packet;
            goto done;
        }
        } /* switch */
    }

done:
    return status;
}

static idigi_callback_status_t connect_server(idigi_data_t * const idigi_ptr, char const * server_url)
{
    idigi_callback_status_t status;
    size_t length;
    idigi_request_t const request_id = {idigi_network_connect};


    status = idigi_callback(idigi_ptr->callback, idigi_class_network, request_id, server_url, strlen(server_url), &idigi_ptr->network_handle, &length);
    switch (status)
    {
    case idigi_callback_continue:
        if (idigi_ptr->network_handle != NULL && length == sizeof *idigi_ptr->network_handle)
        {
            idigi_ptr->edp_connected = true;
            idigi_ptr->network_busy = false;
        }
        else
        {
            idigi_ptr->error_code = (idigi_ptr->network_handle == NULL) ? idigi_invalid_data : idigi_invalid_data_size;
            notify_error_status(idigi_ptr->callback, idigi_class_config, request_id, idigi_ptr->error_code);
            status = idigi_callback_abort;
        }
        break;
    case  idigi_callback_abort:
    case idigi_callback_unrecognized:
        idigi_ptr->error_code = idigi_connect_error;
        break;

    case idigi_callback_busy:
        break;
    }

    return status;
}

static idigi_callback_status_t close_server(idigi_data_t * const idigi_ptr)
{
    idigi_callback_status_t status = idigi_callback_continue;

    if (idigi_ptr->network_handle != NULL)
    {
        idigi_request_t const request_id = {idigi_network_close};

        status = idigi_callback_no_response(idigi_ptr->callback, idigi_class_network, request_id, idigi_ptr->network_handle, sizeof *idigi_ptr->network_handle);
        if (status == idigi_callback_continue || status == idigi_callback_abort)
        {
            idigi_ptr->send_packet.total_length = 0;
            idigi_ptr->receive_packet.index = 0;
            idigi_ptr->network_handle = NULL;
            if (status == idigi_callback_continue)
            {
                idigi_ptr->edp_connected = false;
            }
            if (status == idigi_callback_abort)
            {
                idigi_ptr->error_code = idigi_close_error;
            }
        }
    }
    return status;
}

