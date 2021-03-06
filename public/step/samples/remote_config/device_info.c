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

#include <malloc.h>
#include "idigi_config.h"
#include "idigi_api.h"
#include "platform.h"
#include "remote_config_cb.h"

#define VALUE_TO_STRING(value)   # value
#define MACRO_TO_STRING(macro)   VALUE_TO_STRING(macro)


#define DEVICE_INFO_PRODUCT_LENGTH  64
#define DEVICE_INFO_MODEL_LENGTH  32
#define DEVICE_INFO_COMPANY_LENGTH  64
#define DEVICE_INFO_DESC_LENGTH  128
#define DEVICE_INFO_SYSPWD_LENGTH  32


typedef struct {
    char product[DEVICE_INFO_PRODUCT_LENGTH];
    char model[DEVICE_INFO_MODEL_LENGTH];
    char company[DEVICE_INFO_COMPANY_LENGTH];
    char desc[DEVICE_INFO_DESC_LENGTH];
    size_t desc_length;
} device_info_config_data_t;

device_info_config_data_t device_info_config_data = {"iDigi Connector Product\0", "\0", "Digi International Inc.\0", "iDigi Connector Demo on Linux\n"
        "with firmware upgrade, and remote configuration supports\0", 102};

void print_device_info_desc(void)
{
    printf("%s", device_info_config_data.desc);
}
idigi_callback_status_t app_device_info_group_init(idigi_remote_group_request_t const * const request, idigi_remote_group_response_t * const response)
{

    void * ptr;
    remote_group_session_t * const session_ptr = response->user_context;
    device_info_config_data_t * device_info_ptr = NULL;

    UNUSED_ARGUMENT(request);

    ASSERT(session_ptr != NULL);

    ptr = malloc(sizeof *device_info_ptr);

    if (ptr == NULL)
    {
        response->error_id = idigi_global_error_memory_fail;
        goto done;
    }

    device_info_ptr = ptr;
    *device_info_ptr = device_info_config_data;

    session_ptr->group_context = device_info_ptr;
done:
    return idigi_callback_continue;
}

idigi_callback_status_t app_device_info_group_get(idigi_remote_group_request_t const * const request, idigi_remote_group_response_t * const response)
{
    idigi_callback_status_t status = idigi_callback_continue;
    remote_group_session_t * const session_ptr = response->user_context;


    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    switch (request->element.id)
    {
    case idigi_setting_device_info_version:
        ASSERT(request->element.type == idigi_element_type_0xhex);
        response->element_data.element_value->unsigned_integer_value = IDIGI_VERSION;
        break;
    case idigi_setting_device_info_product:
    case idigi_setting_device_info_model:
    case idigi_setting_device_info_company:
    case idigi_setting_device_info_desc:
    {
        device_info_config_data_t * const device_info_ptr = session_ptr->group_context;

        char * config_data[] = {NULL, device_info_ptr->product, device_info_ptr->model,
                                device_info_ptr->company, device_info_ptr->desc};

        ASSERT(asizeof(config_data) == idigi_setting_device_info_COUNT);

        response->element_data.element_value->string_value = config_data[request->element.id];
        if (request->element.id == idigi_setting_device_info_desc)
        {
            ASSERT(request->element.type == idigi_element_type_multiline_string);
        }
        else
        {
            ASSERT(request->element.type == idigi_element_type_string);
        }
        break;
    }
    default:
        ASSERT(0);
        break;
    }

    return status;
}

idigi_callback_status_t app_device_info_group_set(idigi_remote_group_request_t const * const request, idigi_remote_group_response_t * const response)
{
    idigi_callback_status_t status = idigi_callback_continue;
    remote_group_session_t * const session_ptr = response->user_context;


    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    switch (request->element.id)
    {
    case idigi_setting_device_info_product:
    case idigi_setting_device_info_model:
    case idigi_setting_device_info_company:
    case idigi_setting_device_info_desc:
    {
        device_info_config_data_t * const device_info_ptr = session_ptr->group_context;
        size_t value_length;

        struct {
            char * store_data;
            size_t min_length;
            size_t max_length;
        } config_data[] = {
                {NULL, 0, 0}, /* ignore idigi_setting_device_info_version */
                {device_info_ptr->product, 1, sizeof device_info_ptr->product},
                {device_info_ptr->model, 0, sizeof device_info_ptr->model},
                {device_info_ptr->company, 0, sizeof device_info_ptr->company},
                {device_info_ptr->desc, 0, sizeof device_info_ptr->desc},
        };

        ASSERT(asizeof(config_data) == idigi_setting_device_info_COUNT);

        value_length = strlen(request->element.value->string_value);

        if ((value_length < config_data[request->element.id].min_length) ||
            (value_length >= config_data[request->element.id].max_length))
        {
            #define LENGTH_FORMAT      "%zu"
            #define MINIMUM_HINT_TEXT  "Minimum length is "
            #define MAXIMUM_HINT_TEXT  "Maximum length is "
            {
                static char error_hint_text[sizeof (MINIMUM_HINT_TEXT MACRO_TO_STRING(UINT_MAX))];
                size_t const min_length = config_data[request->element.id].min_length;
                size_t const max_length = config_data[request->element.id].max_length;

                idigi_boolean_t const is_min = (value_length < min_length) ? idigi_boolean_true : idigi_boolean_false;

                CONFIRM(sizeof MINIMUM_HINT_TEXT == sizeof MAXIMUM_HINT_TEXT);

                response->error_id = idigi_setting_device_info_error_invalid_length;
                {
                       static char const min_format[] = MINIMUM_HINT_TEXT LENGTH_FORMAT;
                       static char const max_format[] = MAXIMUM_HINT_TEXT LENGTH_FORMAT;

                snprintf(error_hint_text, sizeof error_hint_text, is_min ? min_format : max_format, is_min? min_length : max_length);
                }
                response->element_data.error_hint = error_hint_text;
            }
            #undef LENGTH_FORMAT
            #undef MINIMUM_HINT_TEXT
            #undef MAXIMUM_HINT_TEXT

            goto done;
        }
        memcpy(config_data[request->element.id].store_data, request->element.value->string_value, value_length);
        config_data[request->element.id].store_data[value_length] = '\0';
        if (request->element.id == idigi_setting_device_info_desc)
        {
            device_info_ptr->desc_length = value_length;
        }
        break;
    }
    default:
        ASSERT(0);
        break;
    }
done:
    return status;
}

idigi_callback_status_t app_device_info_group_end(idigi_remote_group_request_t const * const request, idigi_remote_group_response_t * const response)
{

    device_info_config_data_t * device_info_ptr;
    remote_group_session_t * const session_ptr = response->user_context;

    /* save the data */

    ASSERT(session_ptr != NULL);
    ASSERT(session_ptr->group_context != NULL);

    device_info_ptr = session_ptr->group_context;

    if (request->action == idigi_remote_action_set)
    {
        device_info_config_data = *device_info_ptr;
    }

    if (device_info_ptr != NULL)
    {
        free(device_info_ptr);
    }
    return idigi_callback_continue;
}

void app_device_info_group_cancel(void * const context)
{
    remote_group_session_t * const session_ptr = context;

    if (session_ptr != NULL)
    {
        free(session_ptr->group_context);
    }

}

