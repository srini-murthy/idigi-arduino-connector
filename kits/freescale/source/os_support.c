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
#include <stdlib.h>
#include <main.h>
#include "os_support.h"
#include "idigi_api.h"
#include "platform.h"

static char const * event_list[] =
{
    "idigi_send"
};

static size_t const event_list_size = asizeof(event_list);

#define SEND_EVENT_BIT  0x01    /* TBD */

idigi_connector_error_t ic_create_event(int const event)
{
    idigi_connector_error_t status = idigi_connector_event_error;

    ASSERT_GOTO(event < event_list_size, error);
    if (_event_create(event_list[event]) != MQX_OK)
    {
        APP_DEBUG("Failed to create %s event\n", event_list[event]);
        goto error;
    }

    status = idigi_connector_success;

error:
    return status;
}

idigi_connector_error_t ic_get_event(int const event, unsigned long timeout_ms)
{
    pointer event_ptr;
    idigi_connector_error_t status = idigi_connector_event_error;

    ASSERT_GOTO(event < event_list_size, error);
    if (_event_open(event_list[event], &event_ptr) != MQX_OK)
    {
        APP_DEBUG("Failed to open event(get) %s\n", event_list[event]);
        goto error;
    }

    if (_event_wait_all(event_ptr, SEND_EVENT_BIT, timeout_ms) != MQX_OK)
    {
        APP_DEBUG("Failed to wait on event %s\n", event_list[event]);
        goto error;
    }

    if (_event_clear(event_ptr, SEND_EVENT_BIT) != MQX_OK)
    {
        APP_DEBUG("Failed to clear event %s\n", event_list[event]);
        goto error;
    }

    status = idigi_connector_success;

error:
    return status;
}

idigi_connector_error_t ic_set_event(int const event)
{
    pointer event_ptr;
    idigi_connector_error_t status = idigi_connector_event_error;

    ASSERT_GOTO(event < event_list_size, error);
    if (_event_open(event_list[event], &event_ptr) != MQX_OK)
    {
        APP_DEBUG("Failed to open event(get) %s\n", event_list[event]);
        goto error;
    }

    if (_event_set(event_ptr, SEND_EVENT_BIT) != MQX_OK)
    {
        APP_DEBUG("Failed to clear event %s\n", event_list[event]);
        goto error;
    }

    status = idigi_connector_success;

error:
    return status;
}

void * ic_malloc(size_t size)
{
    return _mem_alloc(size);
}

void ic_free(void * ptr)
{
    if (ptr != NULL)
    {
        _mem_free(ptr);
    }
    else
    {
        APP_DEBUG("Free called with NULL pointer\n");
    }
}

idigi_connector_error_t ic_create_thread(void)
{
    idigi_connector_error_t status = idigi_connector_success;
    _task_id const connector_task_id = _task_create(0, IDIGI_CONNECTOR_TASK, 0);

    if (connector_task_id == MQX_NULL_TASK_ID)
    {
        APP_DEBUG("Failed to create IDIGI_CONNECTOR_TASK\n");
        status = idigi_connector_failed_to_create_thread;
    }

error:
    return status;
}
