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

#define RCI_DEBUG_SHOW_ALL      idigi_true
#define RCI_DEBUG_SHOW_DIFFS    idigi_false

#if !(defined RCI_DEBUG)

#define output_debug_info(rci, show_all)    ((void) rci, (void) show_all)

#else

static void reassign_service_buffer(char * const storage, rci_service_buffer_t * const dst, rci_service_buffer_t * const src)
{
    if ((src->data != NULL) && (src->bytes != 0))
    {
        dst->data = storage;
        dst->bytes = src->bytes;
        memset(dst->data, 0, dst->bytes);
    }
    else
    {
        dst->data = NULL;
        dst->bytes = 0;
    }
}

static char const * boolean_as_string(idigi_bool_t const value)
{
    char const * const result = (value ? "true" : "false");

    return result;
}

static char const * character_as_string(int const value)
{
    static char storage[sizeof "0xFF"][2];
    static size_t index;
    static size_t const count = asizeof(storage);
    char * result;

    switch (value)
    {
    case '\0':  result = "\\0"; break;
    case '\r':  result = "\\r"; break;
    case '\n':  result = "\\n"; break;
    case '\t':  result = "\\t"; break;
    case '\f':  result = "\\f"; break;
    default:
        result = storage[index];
        index++;
        if (index == count) index = 0;

        if (isprint(value))
        {
            result[0] = value;
            result[1] = '\0';
        }
        else
        {
            unsigned char const byte = value;
            sprintf(result, "0x%02x", byte);
        }
        break;
    }

    return result;
}

#define enum_to_case(name)  case name:  result = #name;             break

static char const * rci_status_t_as_string(rci_status_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(rci_status_internal_error);
        enum_to_case(rci_status_complete);
        enum_to_case(rci_status_busy);
        enum_to_case(rci_status_more_input);
        enum_to_case(rci_status_flush_output);
        enum_to_case(rci_status_error);
    }
    return result;
}

static char const * idigi_remote_config_request_t_as_string(idigi_remote_config_request_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(idigi_remote_config_session_start);
        enum_to_case(idigi_remote_config_session_end);
        enum_to_case(idigi_remote_config_action_start);
        enum_to_case(idigi_remote_config_action_end);
        enum_to_case(idigi_remote_config_group_start);
        enum_to_case(idigi_remote_config_group_end);
        enum_to_case(idigi_remote_config_group_process);
        enum_to_case(idigi_remote_config_session_cancel);
    }
    return result;
}

static char const * idigi_callback_status_t_as_string(idigi_callback_status_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(idigi_callback_continue);
        enum_to_case(idigi_callback_busy);
        enum_to_case(idigi_callback_abort);
        enum_to_case(idigi_callback_unrecognized);
    }
    return result;
}

static char const * rci_parser_state_t_as_string(rci_parser_state_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(rci_parser_state_input);
        enum_to_case(rci_parser_state_traversal);
        enum_to_case(rci_parser_state_output);
        enum_to_case(rci_parser_state_error);
    }
    return result;
}

static char const * rci_input_state_t_as_string(rci_input_state_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(rci_input_state_element_tag_open);
        enum_to_case(rci_input_state_element_tag_name);
        enum_to_case(rci_input_state_element_tag_close);
        enum_to_case(rci_input_state_element_start_name);
	    enum_to_case(rci_input_state_element_end_name_first);
	    enum_to_case(rci_input_state_element_end_name);
	    enum_to_case(rci_input_state_element_param_name);
	    enum_to_case(rci_input_state_element_param_equals);
	    enum_to_case(rci_input_state_element_param_quote);
	    enum_to_case(rci_input_state_element_param_value_first);
	    enum_to_case(rci_input_state_element_param_value);
	    enum_to_case(rci_input_state_content_first);
	    enum_to_case(rci_input_state_content);
	    enum_to_case(rci_input_state_content_escaping);
	    enum_to_case(rci_input_state_comment);
    }
    return result;
}

static char const * rci_traversal_state_t_as_string(rci_traversal_state_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(rci_traversal_state_none);
        enum_to_case(rci_traversal_state_all_groups_start);
        enum_to_case(rci_traversal_state_all_groups_group_start);
        enum_to_case(rci_traversal_state_all_groups_element_start);
        enum_to_case(rci_traversal_state_all_groups_element_data);
        enum_to_case(rci_traversal_state_all_groups_element_end);
        enum_to_case(rci_traversal_state_all_groups_group_end);
        enum_to_case(rci_traversal_state_all_groups_group_advance);
        enum_to_case(rci_traversal_state_all_groups_end);
        enum_to_case(rci_traversal_state_one_group_start);
        enum_to_case(rci_traversal_state_one_group_element_start);
        enum_to_case(rci_traversal_state_one_group_element_data);
        enum_to_case(rci_traversal_state_one_group_element_end);
        enum_to_case(rci_traversal_state_one_group_end);
        enum_to_case(rci_traversal_state_one_group_advance);
        enum_to_case(rci_traversal_state_indexed_group_start);
        enum_to_case(rci_traversal_state_indexed_group_element_start);
        enum_to_case(rci_traversal_state_indexed_group_element_data);
        enum_to_case(rci_traversal_state_indexed_group_element_end);
        enum_to_case(rci_traversal_state_indexed_group_end);
        enum_to_case(rci_traversal_state_indexed_group_advance);
        enum_to_case(rci_traversal_state_one_element_start);
        enum_to_case(rci_traversal_state_one_element_data);
        enum_to_case(rci_traversal_state_one_element_end);
    }
    return result;
}

static char const * rci_command_t_as_string(rci_command_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(rci_command_set_setting);
        enum_to_case(rci_command_set_state);
        enum_to_case(rci_command_query_setting);
        enum_to_case(rci_command_query_state);
        enum_to_case(rci_command_unseen);
        enum_to_case(rci_command_header);
        enum_to_case(rci_command_unknown);
    }
    return result;
}

static char const * idigi_remote_action_t_as_string(idigi_remote_action_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(idigi_remote_action_set);
        enum_to_case(idigi_remote_action_query);
    }
    return result;
}

static char const * idigi_remote_group_type_t_as_string(idigi_remote_group_type_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(idigi_remote_group_setting);
        enum_to_case(idigi_remote_group_state);
    }
    return result;
}

static char const * rci_output_state_t_as_string(rci_output_state_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(rci_output_state_none);
        enum_to_case(rci_output_state_element_tag_open);
	    enum_to_case(rci_output_state_element_tag_slash);
        enum_to_case(rci_output_state_element_tag_name);
        enum_to_case(rci_output_state_element_tag_close);
	    enum_to_case(rci_output_state_element_param_space);
	    enum_to_case(rci_output_state_element_param_name);
	    enum_to_case(rci_output_state_element_param_equal_sign);
	    enum_to_case(rci_output_state_element_param_start_quote);
	    enum_to_case(rci_output_state_element_param_value);
	    enum_to_case(rci_output_state_element_param_end_quote);
	    enum_to_case(rci_output_state_content_formatted);
	    enum_to_case(rci_output_state_content);
	    enum_to_case(rci_output_state_content_scan);
	    enum_to_case(rci_output_state_content_entity);
	    enum_to_case(rci_output_state_content_semicolon);
    }
    return result;
}

static char const * rci_output_type_t_as_string(rci_output_type_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(rci_output_type_start_tag);
        enum_to_case(rci_output_type_content);
        enum_to_case(rci_output_type_content_formatted);
        enum_to_case(rci_output_type_end_tag);
        enum_to_case(rci_output_type_unary);
        enum_to_case(rci_output_type_three_tuple);
        enum_to_case(rci_output_type_three_tuple_formatted);
    }
    return result;
}

static char const * idigi_element_value_type_t_as_string(idigi_element_value_type_t const value)
{
    char const * result;
    switch (value)
    {
#if defined RCI_PARSER_USES_STRING
        enum_to_case(idigi_element_type_string);
#endif

#if defined RCI_PARSER_USES_MULTILINE_STRING
        enum_to_case(idigi_element_type_multiline_string);
#endif
#if defined RCI_PARSER_USES_PASSWORD
        enum_to_case(idigi_element_type_password);
#endif
#if defined RCI_PARSER_USES_INT32
        enum_to_case(idigi_element_type_int32);
#endif
#if defined RCI_PARSER_USES_UINT32
        enum_to_case(idigi_element_type_uint32);
#endif
#if defined RCI_PARSER_USES_HEX32
        enum_to_case(idigi_element_type_hex32);
#endif
#if defined RCI_PARSER_USES_0XHEX
        enum_to_case(idigi_element_type_0xhex);
#endif
#if defined RCI_PARSER_USES_FLOAT
        enum_to_case(idigi_element_type_float);
#endif
#if defined RCI_PARSER_USES_ENUM
        enum_to_case(idigi_element_type_enum);
#endif
#if defined RCI_PARSER_USES_ON_OFF
        enum_to_case(idigi_element_type_on_off);
#endif
#if defined RCI_PARSER_USES_BOOLEAN
        enum_to_case(idigi_element_type_boolean);
#endif
#if defined RCI_PARSER_USES_IPV4
        enum_to_case(idigi_element_type_ipv4);
#endif
#if defined RCI_PARSER_USES_FQDNV4
        enum_to_case(idigi_element_type_fqdnv4);
#endif
#if defined RCI_PARSER_USES_FQDNV6
        enum_to_case(idigi_element_type_fqdnv6);
#endif
#if defined RCI_PARSER_USES_DATETIME
        enum_to_case(idigi_element_type_datetime);
#endif
    }
    return result;
}

static char const * rci_error_state_t_as_string(rci_error_state_t const value)
{
    char const * result;
    switch (value)
    {
        enum_to_case(rci_error_state_none);
        enum_to_case(rci_error_state_error_open);
        enum_to_case(rci_error_state_error_description);
        enum_to_case(rci_error_state_error_hint);
        enum_to_case(rci_error_state_error_close);
        enum_to_case(rci_error_state_element_close);
        enum_to_case(rci_error_state_group_close);
        enum_to_case(rci_error_state_command_close);
        enum_to_case(rci_error_state_reply_close);
    }
    return result;
}

static void output_buffer_diff(char const * const name, void const * const current, void * const previous, size_t const bytes)
{
    if (memcmp(current, previous, bytes) != 0)
    {
        unsigned char const * current_buffer = current;
        size_t remaining = bytes;

        idigi_debug_printf("%s\n", name);
        while (remaining != 0)
        {
            size_t const bytes_per_line = 16;
            size_t const bytes_this_line = (remaining < bytes_per_line) ? remaining : bytes_per_line;
            size_t const bytes_extra = (bytes_per_line - bytes_this_line);
            size_t i;

            idigi_debug_printf("%p: ", current_buffer);
            for (i = 0; i < bytes_this_line; i++)
            {
                unsigned char const byte = current_buffer[i];

                idigi_debug_printf("%02x ", byte);
            }
            for (i = 0; i < bytes_extra; i++)
            {
                idigi_debug_printf("   ");
            }
            for (i = 0; i < bytes_this_line; i++)
            {
                int const byte = current_buffer[i];
                int const replacement = '.';

                idigi_debug_printf("%c ", (isprint(byte) && !iscntrl(byte)) ? byte : replacement);
            }

            remaining -= bytes_this_line;
            current_buffer += bytes_this_line;

            putchar('\n');
        }

        memcpy(previous, current, bytes);
    }
}

#define output_diff(format, operation, member) \
    do { \
        if (show_all) \
            idigi_debug_printf("%s(%p): " format "\n", #member, (void *)&(current->member), operation(current->member)); \
        else if (previous.member != current->member) \
        { \
            idigi_debug_printf("%s(%p): " format " -> " format "\n", #member, (void *)&(current->member), operation(previous.member), operation(current->member)); \
            previous.member = current->member; \
        } \
    } while (0)

#define output_enum(name, member)               output_diff("%s", name ## _as_string, member)
#define output_boolean(member)                  output_diff("%s", boolean_as_string, member)
#define output_character(member)                output_diff("%s", character_as_string, member)
#define output_pointer(member)                  output_diff("%p", (void const *), member)
#define output_unsigned_int(member)             output_diff("%u", (unsigned int), member)
#define output_signed_int(member)               output_diff("%d", (signed int), member)
#define output_size(member)                     output_diff("%zu", (size_t), member)
#define output_string(member)                   output_diff("%s", (char const *), member)

#define output_service_data_buffer(name)        do { output_pointer(name.data); output_unsigned_int(name.bytes); } while (0)
#define output_rci_buffer(name)                 do { output_pointer(name.start); output_pointer(name.end); output_pointer(name.current); } while (0)
#define output_rci_string(name)                 do { output_pointer(name.data); output_size(name.length); } while (0)
#define output_rci_attribute(pair)              do { output_rci_string(pair.name); output_rci_string(pair.value); } while (0)

#define output_service_data_buffer_diff(name)   output_buffer_diff("service_data." #name, current->service_data->name.data, service_data.name.data, current->service_data->name.bytes)

static void output_debug_info(rci_t const * const current, idigi_bool_t const show_all)
{
    static rci_t previous;
    static char input_service_data_storage[MSG_MAX_RECV_PACKET_SIZE];
    static char output_service_data_storage[MSG_MAX_SEND_PACKET_SIZE];
    static rci_service_data_t service_data;
    static long unsigned int step;
    idigi_bool_t show_service_data = (current->service_data != NULL);

    /* have we started a new session? */
    if (show_service_data)
    {
        if (previous.service_data == NULL)
        {
            reassign_service_buffer(input_service_data_storage, &service_data.input, &current->service_data->input);
            reassign_service_buffer(output_service_data_storage, &service_data.output, &current->service_data->output);
        }
    }
    else
    {
        previous.service_data = NULL;
    }

    idigi_debug_printf("STEP %lu:\n", step);

    output_pointer(service_data);
    if (show_service_data)
    {
        output_service_data_buffer(service_data->input);
        output_service_data_buffer(service_data->output);
    }

    output_enum(rci_status_t, status);

    output_enum(idigi_remote_config_request_t, callback.request.remote_config_request);
    output_enum(idigi_callback_status_t, callback.status);

    output_rci_buffer(buffer.input);
    output_rci_buffer(buffer.output);

    output_enum(rci_parser_state_t, parser.state.current);
    output_enum(rci_parser_state_t, parser.state.previous);

    output_enum(rci_input_state_t, input.state);
    output_unsigned_int(input.hyphens);
    output_character(input.character);
    output_pointer(input.destination);
    output_enum(rci_command_t, input.command);
    output_rci_string(input.entity);
    output_rci_attribute(input.attribute.match);
    output_rci_attribute(input.attribute.current);

    output_enum(rci_traversal_state_t, traversal.state);

    output_enum(rci_output_state_t, output.state);
    output_enum(rci_output_type_t, output.type);
    output_enum(rci_output_type_t, output.current);
    output_rci_string(output.tag);
    output_rci_attribute(output.attribute);
    output_size(output.entity_scan_index);

    output_enum(rci_error_state_t, error.state);
    output_pointer(error.description);

    output_rci_string(shared.string.generic);
    output_unsigned_int(shared.current.group.id);
    output_unsigned_int(shared.current.group.index);
    output_unsigned_int(shared.current.element.id);
    /* TODO: shared.value */
    output_enum(idigi_remote_action_t, shared.request.action);
    output_enum(idigi_remote_group_type_t, shared.request.group.type);
    output_unsigned_int(shared.request.group.id);
    output_unsigned_int(shared.request.group.index);
    output_unsigned_int(shared.request.element.id);
    /* TODO: shared.request.element.type */
    /* TODO: shared.request.element.value */
    output_pointer(shared.response.user_context);
    output_unsigned_int(shared.response.error_id);
    if (current->shared.response.error_id == 0)
    {
        /* TODO: shared.response.element_data.element_value */
    }
    else
    {
        output_string(shared.response.element_data.error_hint);
    }

    if (show_service_data)
    {
        output_service_data_buffer_diff(input);
        output_service_data_buffer_diff(output);
    }

    /* TODO: output temporary transfer buffer */

    previous = *current;
    step++;
    fflush(stdout);
}

#endif
