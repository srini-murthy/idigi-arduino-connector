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

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "idigi_data.h"

#define ONE_SECOND  1

device_data_t device_data;


idigi_callback_status_t idigi_callback(idigi_class_t class, idigi_request_t request,
                                    void const * request_data, size_t request_length,
                                    void * response_data, size_t * response_length)
{
    idigi_callback_status_t   status = idigi_callback_continue;

    switch (class)
    {
    case idigi_class_config:
        status = idigi_config_callback(request.config_request, request_data, request_length, response_data, response_length);
        break;
    case idigi_class_operating_system:
        status = idigi_os_callback(request.os_request, request_data, request_length, response_data, response_length);
        break;
    case idigi_class_network:
        status = idigi_network_callback(request.network_request, request_data, request_length, response_data, response_length);
        break;
    case idigi_class_firmware:
        status = idigi_firmware_callback(request.firmware_request, request_data, request_length, response_data, response_length);
        break;
    default:
        /* not supported */
        break;
    }
    return status;
}



int main (void)
{
    idigi_status_t status = idigi_success;

    time(&device_data.start_system_up_time);
    device_data.idigi_handle = NULL;
    device_data.select_data = 0;
    device_data.socket_fd = INADDR_NONE;

    DEBUG_PRINTF("Start iDigi\n");
    device_data.idigi_handle = idigi_init((idigi_callback_t) idigi_callback);
    if (device_data.idigi_handle != 0)
    {

        while (status == idigi_success)
        {
            status = idigi_step(device_data.idigi_handle);
            device_data.select_data = 0;

            if (status == idigi_success)
            {
                device_data.select_data |= NETWORK_TIMEOUT_SET | NETWORK_READ_SET;
                network_select(device_data.socket_fd, device_data.select_data, ONE_SECOND);
            }
        }
        DEBUG_PRINTF("idigi status = %d\n", status);
   }
   DEBUG_PRINTF("iDigi stops running!\n");
    return 0;
}

