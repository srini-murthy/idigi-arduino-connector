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
/**
 * @file
 *  @brief iDigi connector remote configuration generated by the iDgii connection remote configuration tool.
 *
 */
#ifndef remote_config_h
#define remote_config_h


/**
 * @defgroup idigi_on_off_t  iDigi Connector on or off values
 * @{
 */
/**
 * iDigi Connector ON and OFF value types
 */
typedef enum {
    idigi_off,  /**< OFF */
    idigi_on    /**< ON */
} idigi_on_off_t;
/**
* @}
*/

/**
* @defgroup idigi_boolean_t iDigi Connector boolean type
* @{
*/
/**
* Boolean type
*/
typedef enum {
    idigi_boolean_false,    /**< False */
    idigi_boolean_true      /**< True */
} idigi_boolean_t;
/**
* @}
*/


/**
 * @defgroup idigi_element_value_type_t iDigi Connector remote configuration element types
 * @{
 */
/**
 * Remote configuration element value types
 */
typedef enum {
    idigi_element_type_string,              /**< Single line string */
    idigi_element_type_multiline_string,    /**< This type is used for a string that may be more
                                               than one line. A "text area" is shown in the iDigi Device
                                               Cloud that allows a user to form multiple lines by pressing enter. */
    idigi_element_type_password,            /**< This Password type is same as string type except it’s shown "*"
                                                when a user types in characters in the iDigi Device Cloud. */
    idigi_element_type_int32,               /**< 32-bit signed integer value */
    idigi_element_type_uint32,              /**< 32-bit unsigned integer value */
    idigi_element_type_hex32,               /**< 32-bit unsigned hexadecimal */
    idigi_element_type_0xhex,               /**< 32-bit unsigned hexadecimal with 0x prefix shown in the iDigi Device Cloud.*/
    idigi_element_type_float,               /**< Floating value. float.h is included. */
    idigi_element_type_enum,                /**< enum is used to define a set of allowed values for an element.
                                                This is a pull-down menu shown in the iDigi Device Cloud. */
    idigi_element_type_on_off,              /**< "on" or "off" value. An applciation's callback should use
                                                 idigi_on value for "on"  or idigi_off value for "off". See @ref idigi_on_off_t*/
    idigi_element_type_boolean,             /**< true or false value. An application's callback should use  idigi_boolean_true
                                                value for true  or idigi_boolean_false value for false. See @ref idigi_boolean_t */

    idigi_element_type_ipv4,                /**< Valid IPv4 address (32-bit value) which is shown aaa.bbb.ccc.ddd in the iDigi Device Cloud */
    idigi_element_type_fqdnv4,              /**< This type accepts either ipv4 or DNS name. */
    idigi_element_type_fqdnv6,              /**< This type accepts either IPv6 address, IPv4 address, or DNS name. */
    idigi_element_type_datetime             /**< Date and time type. It’s string which contains the ISO 8601 standard
                                                 for date and time representation. The format is: YYYY-MM-DDTHH:MM:SStz
                                                 where
                                                     @htmlonly
                                                     <table>
                                                     <tr><td>YYYY:</td> <td>Year<td></tr>
                                                     <tr><td>MM:</td> <td>Month<td></tr>
                                                     <tr><td>DD:</td> <td>Day<td></tr>
                                                     <tr><td>T:</td> <td>The separator between date and time<td></tr>
                                                     <tr><td>HH:</td> <td>Hours in 24-hour clock<td></tr>
                                                     <tr><td>MM:</td> <td>Minutes<td></tr>
                                                     <tr><td>SS:</td> <td>Seconds<td></tr>
                                                     <tr><td>tz:</td> <td>Time zone, specified either in the
                                                         form [+,-]HHMM or Z for Coordinated Universal Time (UTC) <td></tr>
                                                     </table>
                                                    @endhtmlonly */
} idigi_element_value_type_t;
/**
* @}
*/


/**
 * @defgroup idigi_element_value_t iDigi Connector remote configuration element value union
 * @{
 */
/**
 * Remote Configuration Element value
 */
typedef union {
    char const * string_value;          /**< Pointer to a null-terminated string for the following value types:
                                             @htmlonly
                                             <ul>
                                             <li>@endhtmlonly @ref idigi_element_type_string @htmlonly</li>
                                             <li>@endhtmlonly @ref idigi_element_type_multiline_string @htmlonly</li>
                                             <li>@endhtmlonly @ref idigi_element_type_password @htmlonly</li>
                                             <li>@endhtmlonly @ref idigi_element_type_ipv4 @htmlonly</li>
                                             <li>@endhtmlonly @ref idigi_element_type_fqdnv4 @htmlonly</li>
                                             <li>@endhtmlonly @ref idigi_element_type_fqdnv6 @htmlonly</li>
                                             <li>@endhtmlonly @ref idigi_element_type_datetime @htmlonly</li>
                                             </ul>
                                             @endhtmlonly
                                             Note: An application's callback must return the address of the pointer
                                             to the string for idigi_remote_action_set.
                                         */

    int32_t signed_integer_value;       /**< Signed integer value for @ref idigi_element_type_int32 value type */

    uint32_t unsigned_integer_value;    /**< Unsigned integer value for the following value types:
                                             @htmlonly
                                             <ul>
                                             <li>@endhtmlonly @ref idigi_element_type_uint32 @htmlonly</li>
                                             <li>@endhtmlonly @ref idigi_element_type_hex32 @htmlonly</li>
                                             <li>@endhtmlonly @ref idigi_element_type_0xhex @htmlonly</li>
                                             </ul>
                                             @endhtmlonly
                                           */

    double float_value;                 /**< Floating point value for @ref idigi_element_type_float value type */

    unsigned int enum_value;            /**< Enumeration value for @ref idigi_element_type_enum value type */

    idigi_on_off_t  on_off_value;       /**< @ref idigi_on for on or @ref idigi_off for Off value for @ref idigi_element_type_on_off value type */

    idigi_boolean_t boolean_value;      /**< @ref idigi_boolean_true for true or @ref idigi_boolean_false for false value for @ref idigi_element_type_boolean value type */

} idigi_element_value_t;
/**
* @}
*/

#endif /* remote_config_h */
