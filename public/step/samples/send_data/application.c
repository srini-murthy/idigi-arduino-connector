/*
 * Copyright (c) 2011, 2012 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "idigi_api.h"
#include "platform.h"

extern idigi_callback_status_t app_data_service_handler(idigi_data_service_request_t const request,
                                                  void const * const request_data, size_t const request_length,
                                                  void * response_data, size_t * const response_length);

extern idigi_status_t app_send_put_request(idigi_handle_t handle);

idigi_callback_status_t app_idigi_callback(idigi_class_t const class_id, idigi_request_t const request_id,
                                    void const * const request_data, size_t const request_length,
                                    void * response_data, size_t * const response_length)
{
    idigi_callback_status_t   status;

    switch (class_id)
    {
    case idigi_class_config:
        status = app_config_handler(request_id.config_request, request_data, request_length, response_data, response_length);
        break;

    case idigi_class_operating_system:
        status = app_os_handler(request_id.os_request, request_data, request_length, response_data, response_length);
        break;

    case idigi_class_network:
        status = app_network_handler(request_id.network_request, request_data, request_length, response_data, response_length);
        break;

    case idigi_class_data_service:
        status = app_data_service_handler(request_id.data_service_request, request_data, request_length, response_data, response_length);
        break;

    default:
        status = idigi_callback_unrecognized;
        /* not supported */
        break;
    }
    return status;
}

int application_step(idigi_handle_t handle)
{
    static int sent_once = 0;

    if (sent_once == 0) 
    {
        idigi_status_t const status = app_send_put_request(handle);

        switch (status) 
        {
        case idigi_init_error:
            break;

        case idigi_success:
            sent_once = 1;
            break;
        
        default:
            APP_DEBUG("Send data failed [%d]\n", status);
            sent_once = 1;
            break;
        }
    }

    return 0;
}

