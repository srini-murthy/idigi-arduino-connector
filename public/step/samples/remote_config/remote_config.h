/*
 * This is an auto-generated file - DO NOT EDIT! 
 * This is generated by ConfigGenerator tool 
 * This file was generated on: 2012/06/29 13:01:25 
 * The command line arguments were: "-server=test.idigi.com mlchan:***** "Linux Application" 1.0.0.0 config.rci"
 * The version of ConfigGenerator tool was: 1.0.0.0 */

#ifndef remote_config_h
#define remote_config_h

#define RCI_PARSER_USES_ERROR_DESCRIPTIONS
#define RCI_PARSER_USES_STRING
#define RCI_PARSER_USES_MULTILINE_STRING
#define RCI_PARSER_USES_PASSWORD
#define RCI_PARSER_USES_INT32
#define RCI_PARSER_USES_UINT32
#define RCI_PARSER_USES_0XHEX
#define RCI_PARSER_USES_ENUM
#define RCI_PARSER_USES_ON_OFF
#define RCI_PARSER_USES_BOOLEAN
#define RCI_PARSER_USES_IPV4
#define RCI_PARSER_USES_FQDNV4
#define RCI_PARSER_USES_DATETIME
#define RCI_PARSER_USES_UNSIGNED_INTEGER
#define RCI_PARSER_USES_STRINGS

typedef enum {
    idigi_off,
    idigi_on
} idigi_on_off_t;

typedef enum {
    idigi_boolean_false,
    idigi_boolean_true
} idigi_boolean_t;


typedef enum {
    idigi_element_type_string,
    idigi_element_type_multiline_string,
    idigi_element_type_password,
    idigi_element_type_int32,
    idigi_element_type_uint32,
    idigi_element_type_0xhex,
    idigi_element_type_enum,
    idigi_element_type_on_off,
    idigi_element_type_boolean,
    idigi_element_type_ipv4,
    idigi_element_type_fqdnv4,
    idigi_element_type_datetime
} idigi_element_value_type_t;

typedef struct {
    size_t min_length_in_bytes;
    size_t max_length_in_bytes;
} idigi_element_value_string_t;

typedef struct {
   int32_t min_value;
   int32_t max_value;
} idigi_element_value_signed_integer_t;

typedef struct {
   uint32_t min_value;
   uint32_t max_value;
} idigi_element_value_unsigned_integer_t;

typedef struct {
    size_t count;
    char const * const * value;
} idigi_element_value_enum_t;


typedef union {
    char const * string_value;
    int32_t signed_integer_value;
    uint32_t unsigned_integer_value;
    unsigned int enum_value;
    idigi_on_off_t  on_off_value;
    idigi_boolean_t  boolean_value;
} idigi_element_value_t;


 typedef union {
    idigi_element_value_string_t string_value;
    idigi_element_value_signed_integer_t signed_integer_value;
    idigi_element_value_unsigned_integer_t unsigned_integer_value;
    idigi_element_value_enum_t enum_value;
} idigi_element_value_limit_t;


#include "idigi_remote.h"

extern char const idigi_remote_all_strings[];

#define RCI_ENTITY_QUOTE (idigi_remote_all_strings+0)
#define RCI_ENTITY_AMPERSAND (idigi_remote_all_strings+5)
#define RCI_ENTITY_APOSTROPHE (idigi_remote_all_strings+9)
#define RCI_ENTITY_LESS_THAN (idigi_remote_all_strings+14)
#define RCI_ENTITY_GREATER_THAN (idigi_remote_all_strings+17)
#define RCI_SET_SETTING (idigi_remote_all_strings+20)
#define RCI_QUERY_SETTING (idigi_remote_all_strings+32)
#define RCI_SET_STATE (idigi_remote_all_strings+46)
#define RCI_QUERY_STATE (idigi_remote_all_strings+56)
#define RCI_VERSION (idigi_remote_all_strings+68)
#define RCI_VERSION_SUPPORTED (idigi_remote_all_strings+76)
#define RCI_REPLY (idigi_remote_all_strings+80)
#define RCI_REQUEST (idigi_remote_all_strings+90)
#define RCI_INDEX (idigi_remote_all_strings+102)
#define RCI_ID (idigi_remote_all_strings+108)
#define RCI_HINT (idigi_remote_all_strings+111)
#define RCI_ERROR (idigi_remote_all_strings+116)
#define RCI_COMMENT (idigi_remote_all_strings+122)
#define RCI_DESC (idigi_remote_all_strings+126)
#define RCI_ON (idigi_remote_all_strings+131)
#define RCI_OFF (idigi_remote_all_strings+134)
#define RCI_TRUE (idigi_remote_all_strings+138)
#define RCI_FALSE (idigi_remote_all_strings+143)

typedef enum {
 idigi_rci_error_OFFSET = 1,
 idigi_rci_error_parser_error =  idigi_rci_error_OFFSET,
 idigi_rci_error_bad_xml,
 idigi_rci_error_bad_command,
 idigi_rci_error_invalid_version,
 idigi_rci_error_bad_group,
 idigi_rci_error_bad_index,
 idigi_rci_error_bad_element,
 idigi_rci_error_bad_value,
 idigi_rci_error_COUNT
} idigi_rci_error_id_t;


typedef enum {
 idigi_global_error_OFFSET = idigi_rci_error_COUNT,
 idigi_global_error_load_fail =  idigi_global_error_OFFSET,
 idigi_global_error_save_fail,
 idigi_global_error_memory_fail,
 idigi_global_error_COUNT
} idigi_global_error_id_t;

typedef enum {
 idigi_setting_serial_baud_2400,
 idigi_setting_serial_baud_4800,
 idigi_setting_serial_baud_9600,
 idigi_setting_serial_baud_19200,
 idigi_setting_serial_baud_38400,
 idigi_setting_serial_baud_57600,
 idigi_setting_serial_baud_115200,
 idigi_setting_serial_baud_230400,
 idigi_setting_serial_baud_COUNT
} idigi_setting_serial_baud_id_t;

typedef enum {
 idigi_setting_serial_parity_none,
 idigi_setting_serial_parity_odd,
 idigi_setting_serial_parity_even,
 idigi_setting_serial_parity_COUNT
} idigi_setting_serial_parity_id_t;

typedef enum {
 idigi_setting_serial_baud,
 idigi_setting_serial_parity,
 idigi_setting_serial_databits,
 idigi_setting_serial_xbreak,
 idigi_setting_serial_txbytes,
 idigi_setting_serial_COUNT
} idigi_setting_serial_id_t;

typedef enum {
 idigi_setting_serial_error_invalid_baud = idigi_global_error_COUNT,
 idigi_setting_serial_error_invalid_databits,
 idigi_setting_serial_error_invalid_parity,
 idigi_setting_serial_error_invalid_xbreak,
 idigi_setting_serial_error_invalid_databits_parity,
 idigi_setting_serial_error_COUNT
} idigi_setting_serial_error_id_t;

typedef enum {
 idigi_setting_ethernet_duplex_auto,
 idigi_setting_ethernet_duplex_half,
 idigi_setting_ethernet_duplex_full,
 idigi_setting_ethernet_duplex_COUNT
} idigi_setting_ethernet_duplex_id_t;

typedef enum {
 idigi_setting_ethernet_ip,
 idigi_setting_ethernet_subnet,
 idigi_setting_ethernet_gateway,
 idigi_setting_ethernet_dhcp,
 idigi_setting_ethernet_dns,
 idigi_setting_ethernet_duplex,
 idigi_setting_ethernet_COUNT
} idigi_setting_ethernet_id_t;

typedef enum {
 idigi_setting_ethernet_error_invalid_duplex = idigi_global_error_COUNT,
 idigi_setting_ethernet_error_invalid_ip,
 idigi_setting_ethernet_error_invalid_subnet,
 idigi_setting_ethernet_error_invalid_gateway,
 idigi_setting_ethernet_error_invalid_dns,
 idigi_setting_ethernet_error_COUNT
} idigi_setting_ethernet_error_id_t;

typedef enum {
 idigi_setting_device_time_curtime,
 idigi_setting_device_time_signed_integer,
 idigi_setting_device_time_COUNT
} idigi_setting_device_time_id_t;

typedef enum {
 idigi_setting_device_time_error_invalid_time = idigi_global_error_COUNT,
 idigi_setting_device_time_error_invalid_integer,
 idigi_setting_device_time_error_COUNT
} idigi_setting_device_time_error_id_t;

typedef enum {
 idigi_setting_device_info_version,
 idigi_setting_device_info_product,
 idigi_setting_device_info_model,
 idigi_setting_device_info_company,
 idigi_setting_device_info_desc,
 idigi_setting_device_info_COUNT
} idigi_setting_device_info_id_t;

typedef enum {
 idigi_setting_device_info_error_invalid_length = idigi_global_error_COUNT,
 idigi_setting_device_info_error_COUNT
} idigi_setting_device_info_error_id_t;

typedef enum {
 idigi_setting_system_description,
 idigi_setting_system_contact,
 idigi_setting_system_location,
 idigi_setting_system_COUNT
} idigi_setting_system_id_t;

typedef enum {
 idigi_setting_system_error_invalid_length = idigi_global_error_COUNT,
 idigi_setting_system_error_COUNT
} idigi_setting_system_error_id_t;

typedef enum {
 idigi_setting_devicesecurity_identityVerificationForm_simple,
 idigi_setting_devicesecurity_identityVerificationForm_password,
 idigi_setting_devicesecurity_identityVerificationForm_COUNT
} idigi_setting_devicesecurity_identityVerificationForm_id_t;

typedef enum {
 idigi_setting_devicesecurity_identityVerificationForm,
 idigi_setting_devicesecurity_password,
 idigi_setting_devicesecurity_COUNT
} idigi_setting_devicesecurity_id_t;

typedef enum {
 idigi_setting_serial,
 idigi_setting_ethernet,
 idigi_setting_device_time,
 idigi_setting_device_info,
 idigi_setting_system,
 idigi_setting_devicesecurity,
 idigi_setting_COUNT
} idigi_setting_id_t;

typedef enum {
 idigi_state_gps_stats_latitude,
 idigi_state_gps_stats_longitude,
 idigi_state_gps_stats_COUNT
} idigi_state_gps_stats_id_t;

typedef enum {
 idigi_state_gps_stats,
 idigi_state_COUNT
} idigi_state_id_t;



#if defined IDIGI_RCI_PARSER_INTERNAL_DATA

#define FIRMWARE_TARGET_ZERO_VERSION  0x1000000

#define SETTING_SERIAL (idigi_remote_all_strings+149)
#define SETTING_SERIAL_BAUD (idigi_remote_all_strings+156)
#define SETTING_SERIAL_BAUD_2400 (idigi_remote_all_strings+161)
#define SETTING_SERIAL_BAUD_4800 (idigi_remote_all_strings+166)
#define SETTING_SERIAL_BAUD_9600 (idigi_remote_all_strings+171)
#define SETTING_SERIAL_BAUD_19200 (idigi_remote_all_strings+176)
#define SETTING_SERIAL_BAUD_38400 (idigi_remote_all_strings+182)
#define SETTING_SERIAL_BAUD_57600 (idigi_remote_all_strings+188)
#define SETTING_SERIAL_BAUD_115200 (idigi_remote_all_strings+194)
#define SETTING_SERIAL_BAUD_230400 (idigi_remote_all_strings+201)
#define SETTING_SERIAL_PARITY (idigi_remote_all_strings+208)
#define SETTING_SERIAL_PARITY_NONE (idigi_remote_all_strings+215)
#define SETTING_SERIAL_PARITY_ODD (idigi_remote_all_strings+220)
#define SETTING_SERIAL_PARITY_EVEN (idigi_remote_all_strings+224)
#define SETTING_SERIAL_DATABITS (idigi_remote_all_strings+229)
#define SETTING_SERIAL_XBREAK (idigi_remote_all_strings+238)
#define SETTING_SERIAL_TXBYTES (idigi_remote_all_strings+245)
#define SETTING_SERIAL_ERROR_INVALID_BAUD (idigi_remote_all_strings+253)
#define SETTING_SERIAL_ERROR_INVALID_DATABITS (idigi_remote_all_strings+272)
#define SETTING_SERIAL_ERROR_INVALID_PARITY (idigi_remote_all_strings+290)
#define SETTING_SERIAL_ERROR_INVALID_XBREAK (idigi_remote_all_strings+306)
#define SETTING_SERIAL_ERROR_INVALID_DATABITS_PARITY (idigi_remote_all_strings+329)
#define SETTING_ETHERNET (idigi_remote_all_strings+373)
#define SETTING_ETHERNET_IP (idigi_remote_all_strings+382)
#define SETTING_ETHERNET_SUBNET (idigi_remote_all_strings+385)
#define SETTING_ETHERNET_GATEWAY (idigi_remote_all_strings+392)
#define SETTING_ETHERNET_DHCP (idigi_remote_all_strings+400)
#define SETTING_ETHERNET_DNS (idigi_remote_all_strings+405)
#define SETTING_ETHERNET_DUPLEX (idigi_remote_all_strings+409)
#define SETTING_ETHERNET_DUPLEX_AUTO (idigi_remote_all_strings+416)
#define SETTING_ETHERNET_DUPLEX_HALF (idigi_remote_all_strings+421)
#define SETTING_ETHERNET_DUPLEX_FULL (idigi_remote_all_strings+426)
#define SETTING_ETHERNET_ERROR_INVALID_DUPLEX (idigi_remote_all_strings+431)
#define SETTING_ETHERNET_ERROR_INVALID_IP (idigi_remote_all_strings+463)
#define SETTING_ETHERNET_ERROR_INVALID_SUBNET (idigi_remote_all_strings+482)
#define SETTING_ETHERNET_ERROR_INVALID_GATEWAY (idigi_remote_all_strings+502)
#define SETTING_ETHERNET_ERROR_INVALID_DNS (idigi_remote_all_strings+526)
#define SETTING_DEVICE_TIME (idigi_remote_all_strings+546)
#define SETTING_DEVICE_TIME_CURTIME (idigi_remote_all_strings+558)
#define SETTING_DEVICE_TIME_SIGNED_INTEGER (idigi_remote_all_strings+566)
#define SETTING_DEVICE_TIME_ERROR_INVALID_TIME (idigi_remote_all_strings+581)
#define SETTING_DEVICE_TIME_ERROR_INVALID_INTEGER (idigi_remote_all_strings+594)
#define SETTING_DEVICE_INFO (idigi_remote_all_strings+616)
#define SETTING_DEVICE_INFO_VERSION (idigi_remote_all_strings+628)
#define SETTING_DEVICE_INFO_PRODUCT (idigi_remote_all_strings+636)
#define SETTING_DEVICE_INFO_MODEL (idigi_remote_all_strings+644)
#define SETTING_DEVICE_INFO_COMPANY (idigi_remote_all_strings+650)
#define SETTING_DEVICE_INFO_DESC (idigi_remote_all_strings+658)
#define SETTING_DEVICE_INFO_ERROR_INVALID_LENGTH (idigi_remote_all_strings+663)
#define SETTING_SYSTEM (idigi_remote_all_strings+678)
#define SETTING_SYSTEM_DESCRIPTION (idigi_remote_all_strings+685)
#define SETTING_SYSTEM_CONTACT (idigi_remote_all_strings+697)
#define SETTING_SYSTEM_LOCATION (idigi_remote_all_strings+705)
#define SETTING_SYSTEM_ERROR_INVALID_LENGTH (idigi_remote_all_strings+714)
#define SETTING_DEVICESECURITY (idigi_remote_all_strings+729)
#define SETTING_DEVICESECURITY_IDENTITYVERIFICATIONFORM (idigi_remote_all_strings+744)
#define SETTING_DEVICESECURITY_IDENTITYVERIFICATIONFORM_SIMPLE (idigi_remote_all_strings+769)
#define SETTING_DEVICESECURITY_IDENTITYVERIFICATIONFORM_PASSWORD (idigi_remote_all_strings+776)
#define SETTING_DEVICESECURITY_PASSWORD (idigi_remote_all_strings+785)
#define STATE_GPS_STATS (idigi_remote_all_strings+794)
#define STATE_GPS_STATS_LATITUDE (idigi_remote_all_strings+804)
#define STATE_GPS_STATS_LONGITUDE (idigi_remote_all_strings+813)
#define IDIGI_GLOBAL_ERROR_LOAD_FAIL (idigi_remote_all_strings+823)
#define IDIGI_GLOBAL_ERROR_SAVE_FAIL (idigi_remote_all_strings+833)
#define IDIGI_GLOBAL_ERROR_MEMORY_FAIL (idigi_remote_all_strings+843)
#define IDIGI_RCI_ERROR_PARSER_ERROR (idigi_remote_all_strings+863)
#define IDIGI_RCI_ERROR_BAD_XML (idigi_remote_all_strings+876)
#define IDIGI_RCI_ERROR_BAD_COMMAND (idigi_remote_all_strings+884)
#define IDIGI_RCI_ERROR_INVALID_VERSION (idigi_remote_all_strings+896)
#define IDIGI_RCI_ERROR_BAD_GROUP (idigi_remote_all_strings+912)
#define IDIGI_RCI_ERROR_BAD_INDEX (idigi_remote_all_strings+922)
#define IDIGI_RCI_ERROR_BAD_ELEMENT (idigi_remote_all_strings+932)
#define IDIGI_RCI_ERROR_BAD_VALUE (idigi_remote_all_strings+944)

char const idigi_remote_all_strings[] = {
 4,'q','u','o','t',
 3,'a','m','p',
 4,'a','p','o','s',
 2,'l','t',
 2,'g','t',
 11,'s','e','t','_','s','e','t','t','i','n','g',
 13,'q','u','e','r','y','_','s','e','t','t','i','n','g',
 9,'s','e','t','_','s','t','a','t','e',
 11,'q','u','e','r','y','_','s','t','a','t','e',
 7,'v','e','r','s','i','o','n',
 3,'1','.','1',
 9,'r','c','i','_','r','e','p','l','y',
 11,'r','c','i','_','r','e','q','u','e','s','t',
 5,'i','n','d','e','x',
 2,'i','d',
 4,'h','i','n','t',
 5,'e','r','r','o','r',
 3,'!','-','-',
 4,'d','e','s','c',
 2,'o','n',
 3,'o','f','f',
 4,'t','r','u','e',
 5,'f','a','l','s','e',
 6,'s','e','r','i','a','l',
 4,'b','a','u','d',
 4,'2','4','0','0',
 4,'4','8','0','0',
 4,'9','6','0','0',
 5,'1','9','2','0','0',
 5,'3','8','4','0','0',
 5,'5','7','6','0','0',
 6,'1','1','5','2','0','0',
 6,'2','3','0','4','0','0',
 6,'p','a','r','i','t','y',
 4,'n','o','n','e',
 3,'o','d','d',
 4,'e','v','e','n',
 8,'d','a','t','a','b','i','t','s',
 6,'x','b','r','e','a','k',
 7,'t','x','b','y','t','e','s',
 18,'I','n','v','a','l','i','d',' ','b','a','u','d',' ','r','a','t','e',' ',
 17,'I','n','v','a','l','i','d',' ','d','a','t','a',' ','b','i','t','s',
 15,' ','I','n','v','a','l','i','d',' ','p','a','r','i','t','y',
 22,'I','n','v','a','l','i','d',' ','x','b','r','e','a','k',' ','s','e','t','t','i','n','g',
 43,'I','n','v','a','l','i','d',' ','c','o','m','b','i','n','a','t','i','o','n',' ','o','f',' ','d','a','t','a',' ','b','i','t','s',' ','a','n','d',' ','p','a','r','i','t','y',
 8,'e','t','h','e','r','n','e','t',
 2,'i','p',
 6,'s','u','b','n','e','t',
 7,'g','a','t','e','w','a','y',
 4,'d','h','c','p',
 3,'d','n','s',
 6,'d','u','p','l','e','x',
 4,'a','u','t','o',
 4,'h','a','l','f',
 4,'f','u','l','l',
 31,'I','n','v','a','l','i','d',' ','e','t','h','e','r','n','e','t',' ','d','u','p','l','e','x',' ','s','e','t','t','i','n','g',
 18,'I','n','v','a','l','i','d',' ','I','P',' ','a','d','d','r','e','s','s',
 19,'I','n','v','a','l','i','d',' ','s','u','b','n','e','t',' ','m','a','s','k',
 23,'I','n','v','a','l','i','d',' ','g','a','t','e','w','a','y',' ','a','d','d','r','e','s','s',
 19,'I','n','v','a','l','i','d',' ','D','N','S',' ','a','d','d','r','e','s','s',
 11,'d','e','v','i','c','e','_','t','i','m','e',
 7,'c','u','r','t','i','m','e',
 14,'s','i','g','n','e','d','_','i','n','t','e','g','e','r',
 12,'I','n','v','a','l','i','d',' ','t','i','m','e',
 21,'I','n','v','a','l','i','d',' ','i','n','t','e','g','e','r',' ','v','a','l','u','e',
 11,'d','e','v','i','c','e','_','i','n','f','o',
 7,'v','e','r','s','i','o','n',
 7,'p','r','o','d','u','c','t',
 5,'m','o','d','e','l',
 7,'c','o','m','p','a','n','y',
 4,'d','e','s','c',
 14,'i','n','v','a','l','i','d',' ','l','e','n','g','t','h',
 6,'s','y','s','t','e','m',
 11,'d','e','s','c','r','i','p','t','i','o','n',
 7,'c','o','n','t','a','c','t',
 8,'l','o','c','a','t','i','o','n',
 14,'I','n','v','a','l','i','d',' ','L','e','n','g','t','h',
 14,'d','e','v','i','c','e','s','e','c','u','r','i','t','y',
 24,'i','d','e','n','t','i','t','y','V','e','r','i','f','i','c','a','t','i','o','n','F','o','r','m',
 6,'s','i','m','p','l','e',
 8,'p','a','s','s','w','o','r','d',
 8,'p','a','s','s','w','o','r','d',
 9,'g','p','s','_','s','t','a','t','s',
 8,'l','a','t','i','t','u','d','e',
 9,'l','o','n','g','i','t','u','d','e',
 9,'L','o','a','d',' ','f','a','i','l',
 9,'S','a','v','e',' ','f','a','i','l',
 19,'I','n','s','u','f','f','i','c','i','e','n','t',' ','m','e','m','o','r','y',
 12,'P','a','r','s','e','r',' ','e','r','r','o','r',
 7,'B','a','d',' ','X','M','L',
 11,'B','a','d',' ','c','o','m','m','a','n','d',
 15,'I','n','v','a','l','i','d',' ','v','e','r','s','i','o','n',
 9,'B','a','d',' ','g','r','o','u','p',
 9,'B','a','d',' ','i','n','d','e','x',
 11,'B','a','d',' ','e','l','e','m','e','n','t',
 9,'B','a','d',' ','v','a','l','u','e'
};

static char const * const setting_serial_baud_enum[] = {
 SETTING_SERIAL_BAUD_2400, /*2400*/
 SETTING_SERIAL_BAUD_4800, /*4800*/
 SETTING_SERIAL_BAUD_9600, /*9600*/
 SETTING_SERIAL_BAUD_19200, /*19200*/
 SETTING_SERIAL_BAUD_38400, /*38400*/
 SETTING_SERIAL_BAUD_57600, /*57600*/
 SETTING_SERIAL_BAUD_115200, /*115200*/
 SETTING_SERIAL_BAUD_230400 /*230400*/
};

static idigi_element_value_enum_t const setting_serial_baud_limit = {
 asizeof(setting_serial_baud_enum),
 setting_serial_baud_enum
};

static char const * const setting_serial_parity_enum[] = {
 SETTING_SERIAL_PARITY_NONE, /*none*/
 SETTING_SERIAL_PARITY_ODD, /*odd*/
 SETTING_SERIAL_PARITY_EVEN /*even*/
};

static idigi_element_value_enum_t const setting_serial_parity_limit = {
 asizeof(setting_serial_parity_enum),
 setting_serial_parity_enum
};

static idigi_element_value_unsigned_integer_t const setting_serial_databits_limit = {
 5,
 8
};

static idigi_group_element_t const setting_serial_elements[] = {
 { SETTING_SERIAL_BAUD,  /*baud*/
   idigi_element_access_read_write,
   idigi_element_type_enum,
  (idigi_element_value_limit_t *)&setting_serial_baud_limit
 },
 { SETTING_SERIAL_PARITY,  /*parity*/
   idigi_element_access_read_write,
   idigi_element_type_enum,
  (idigi_element_value_limit_t *)&setting_serial_parity_limit
 },
 { SETTING_SERIAL_DATABITS,  /*databits*/
   idigi_element_access_read_write,
   idigi_element_type_uint32,
  (idigi_element_value_limit_t *)&setting_serial_databits_limit
 },
 { SETTING_SERIAL_XBREAK,  /*xbreak*/
   idigi_element_access_read_write,
   idigi_element_type_on_off,
  NULL
 },
 { SETTING_SERIAL_TXBYTES,  /*txbytes*/
   idigi_element_access_read_only,
   idigi_element_type_uint32,
  NULL
 }
};

static char const * const setting_serial_errors[] = {
 SETTING_SERIAL_ERROR_INVALID_BAUD, /*invalid_baud*/
 SETTING_SERIAL_ERROR_INVALID_DATABITS, /*invalid_databits*/
 SETTING_SERIAL_ERROR_INVALID_PARITY, /*invalid_parity*/
 SETTING_SERIAL_ERROR_INVALID_XBREAK, /*invalid_xbreak*/
 SETTING_SERIAL_ERROR_INVALID_DATABITS_PARITY /*invalid_databits_parity*/
};

static idigi_element_value_string_t const setting_ethernet_dns_limit = {
 0,
 127
};

static char const * const setting_ethernet_duplex_enum[] = {
 SETTING_ETHERNET_DUPLEX_AUTO, /*auto*/
 SETTING_ETHERNET_DUPLEX_HALF, /*half*/
 SETTING_ETHERNET_DUPLEX_FULL /*full*/
};

static idigi_element_value_enum_t const setting_ethernet_duplex_limit = {
 asizeof(setting_ethernet_duplex_enum),
 setting_ethernet_duplex_enum
};

static idigi_group_element_t const setting_ethernet_elements[] = {
 { SETTING_ETHERNET_IP,  /*ip*/
   idigi_element_access_read_write,
   idigi_element_type_ipv4,
  NULL
 },
 { SETTING_ETHERNET_SUBNET,  /*subnet*/
   idigi_element_access_read_write,
   idigi_element_type_ipv4,
  NULL
 },
 { SETTING_ETHERNET_GATEWAY,  /*gateway*/
   idigi_element_access_read_write,
   idigi_element_type_ipv4,
  NULL
 },
 { SETTING_ETHERNET_DHCP,  /*dhcp*/
   idigi_element_access_read_write,
   idigi_element_type_boolean,
  NULL
 },
 { SETTING_ETHERNET_DNS,  /*dns*/
   idigi_element_access_read_write,
   idigi_element_type_fqdnv4,
  (idigi_element_value_limit_t *)&setting_ethernet_dns_limit
 },
 { SETTING_ETHERNET_DUPLEX,  /*duplex*/
   idigi_element_access_read_write,
   idigi_element_type_enum,
  (idigi_element_value_limit_t *)&setting_ethernet_duplex_limit
 }
};

static char const * const setting_ethernet_errors[] = {
 SETTING_ETHERNET_ERROR_INVALID_DUPLEX, /*invalid_duplex*/
 SETTING_ETHERNET_ERROR_INVALID_IP, /*invalid_ip*/
 SETTING_ETHERNET_ERROR_INVALID_SUBNET, /*invalid_subnet*/
 SETTING_ETHERNET_ERROR_INVALID_GATEWAY, /*invalid_gateway*/
 SETTING_ETHERNET_ERROR_INVALID_DNS /*invalid_dns*/
};

static idigi_element_value_signed_integer_t const setting_device_time_signed_integer_limit = {
 -100,
 100
};

static idigi_group_element_t const setting_device_time_elements[] = {
 { SETTING_DEVICE_TIME_CURTIME,  /*curtime*/
   idigi_element_access_read_write,
   idigi_element_type_datetime,
  NULL
 },
 { SETTING_DEVICE_TIME_SIGNED_INTEGER,  /*signed_integer*/
   idigi_element_access_read_write,
   idigi_element_type_int32,
  (idigi_element_value_limit_t *)&setting_device_time_signed_integer_limit
 }
};

static char const * const setting_device_time_errors[] = {
 SETTING_DEVICE_TIME_ERROR_INVALID_TIME, /*invalid_time*/
 SETTING_DEVICE_TIME_ERROR_INVALID_INTEGER /*invalid_integer*/
};

static idigi_element_value_string_t const setting_device_info_product_limit = {
 1,
 64
};

static idigi_element_value_string_t const setting_device_info_model_limit = {
 0,
 32
};

static idigi_group_element_t const setting_device_info_elements[] = {
 { SETTING_DEVICE_INFO_VERSION,  /*version*/
   idigi_element_access_read_only,
   idigi_element_type_0xhex,
  NULL
 },
 { SETTING_DEVICE_INFO_PRODUCT,  /*product*/
   idigi_element_access_read_write,
   idigi_element_type_string,
  (idigi_element_value_limit_t *)&setting_device_info_product_limit
 },
 { SETTING_DEVICE_INFO_MODEL,  /*model*/
   idigi_element_access_read_write,
   idigi_element_type_string,
  (idigi_element_value_limit_t *)&setting_device_info_model_limit
 },
 { SETTING_DEVICE_INFO_COMPANY,  /*company*/
   idigi_element_access_read_write,
   idigi_element_type_string,
  NULL
 },
 { SETTING_DEVICE_INFO_DESC,  /*desc*/
   idigi_element_access_read_write,
   idigi_element_type_multiline_string,
  NULL
 }
};

static char const * const setting_device_info_errors[] = {
 SETTING_DEVICE_INFO_ERROR_INVALID_LENGTH /*invalid_length*/
};

static idigi_element_value_string_t const setting_system_description_limit = {
 0,
 63
};

static idigi_element_value_string_t const setting_system_contact_limit = {
 0,
 63
};

static idigi_element_value_string_t const setting_system_location_limit = {
 0,
 63
};

static idigi_group_element_t const setting_system_elements[] = {
 { SETTING_SYSTEM_DESCRIPTION,  /*description*/
   idigi_element_access_read_write,
   idigi_element_type_string,
  (idigi_element_value_limit_t *)&setting_system_description_limit
 },
 { SETTING_SYSTEM_CONTACT,  /*contact*/
   idigi_element_access_read_write,
   idigi_element_type_string,
  (idigi_element_value_limit_t *)&setting_system_contact_limit
 },
 { SETTING_SYSTEM_LOCATION,  /*location*/
   idigi_element_access_read_write,
   idigi_element_type_string,
  (idigi_element_value_limit_t *)&setting_system_location_limit
 }
};

static char const * const setting_system_errors[] = {
 SETTING_SYSTEM_ERROR_INVALID_LENGTH /*invalid_length*/
};

static char const * const setting_devicesecurity_identityverificationform_enum[] = {
 SETTING_DEVICESECURITY_IDENTITYVERIFICATIONFORM_SIMPLE, /*simple*/
 SETTING_DEVICESECURITY_IDENTITYVERIFICATIONFORM_PASSWORD /*password*/
};

static idigi_element_value_enum_t const setting_devicesecurity_identityverificationform_limit = {
 asizeof(setting_devicesecurity_identityverificationform_enum),
 setting_devicesecurity_identityverificationform_enum
};

static idigi_element_value_string_t const setting_devicesecurity_password_limit = {
 0,
 133
};

static idigi_group_element_t const setting_devicesecurity_elements[] = {
 { SETTING_DEVICESECURITY_IDENTITYVERIFICATIONFORM,  /*identityVerificationForm*/
   idigi_element_access_read_write,
   idigi_element_type_enum,
  (idigi_element_value_limit_t *)&setting_devicesecurity_identityverificationform_limit
 },
 { SETTING_DEVICESECURITY_PASSWORD,  /*password*/
   idigi_element_access_read_write,
   idigi_element_type_password,
  (idigi_element_value_limit_t *)&setting_devicesecurity_password_limit
 }
};

static idigi_group_t const idigi_setting_groups[] = {
 { SETTING_SERIAL,  /*serial*/
   2,
   { asizeof(setting_serial_elements),
     setting_serial_elements
   },
   { asizeof(setting_serial_errors),
     setting_serial_errors
   }
 }
,
 { SETTING_ETHERNET,  /*ethernet*/
   1,
   { asizeof(setting_ethernet_elements),
     setting_ethernet_elements
   },
   { asizeof(setting_ethernet_errors),
     setting_ethernet_errors
   }
 }
,
 { SETTING_DEVICE_TIME,  /*device_time*/
   1,
   { asizeof(setting_device_time_elements),
     setting_device_time_elements
   },
   { asizeof(setting_device_time_errors),
     setting_device_time_errors
   }
 }
,
 { SETTING_DEVICE_INFO,  /*device_info*/
   1,
   { asizeof(setting_device_info_elements),
     setting_device_info_elements
   },
   { asizeof(setting_device_info_errors),
     setting_device_info_errors
   }
 }
,
 { SETTING_SYSTEM,  /*system*/
   1,
   { asizeof(setting_system_elements),
     setting_system_elements
   },
   { asizeof(setting_system_errors),
     setting_system_errors
   }
 }
,
 { SETTING_DEVICESECURITY,  /*devicesecurity*/
   1,
   { asizeof(setting_devicesecurity_elements),
     setting_devicesecurity_elements
   },
   { 0,
     NULL
   }
 }
};

static idigi_group_element_t const state_gps_stats_elements[] = {
 { STATE_GPS_STATS_LATITUDE,  /*latitude*/
   idigi_element_access_read_only,
   idigi_element_type_string,
  NULL
 },
 { STATE_GPS_STATS_LONGITUDE,  /*longitude*/
   idigi_element_access_read_only,
   idigi_element_type_string,
  NULL
 }
};

static idigi_group_t const idigi_state_groups[] = {
 { STATE_GPS_STATS,  /*gps_stats*/
   1,
   { asizeof(state_gps_stats_elements),
     state_gps_stats_elements
   },
   { 0,
     NULL
   }
 }
};

static char const * const idigi_rci_errors[] = {
 IDIGI_RCI_ERROR_PARSER_ERROR, /*parser_error*/
 IDIGI_RCI_ERROR_BAD_XML, /*bad_xml*/
 IDIGI_RCI_ERROR_BAD_COMMAND, /*bad_command*/
 IDIGI_RCI_ERROR_INVALID_VERSION, /*invalid_version*/
 IDIGI_RCI_ERROR_BAD_GROUP, /*bad_group*/
 IDIGI_RCI_ERROR_BAD_INDEX, /*bad_index*/
 IDIGI_RCI_ERROR_BAD_ELEMENT, /*bad_element*/
 IDIGI_RCI_ERROR_BAD_VALUE, /*bad_value*/
 IDIGI_GLOBAL_ERROR_LOAD_FAIL, /*load_fail*/
 IDIGI_GLOBAL_ERROR_SAVE_FAIL, /*save_fail*/
 IDIGI_GLOBAL_ERROR_MEMORY_FAIL /*memory_fail*/
};

static idigi_group_table_t const idigi_group_table[] = {
 {idigi_setting_groups,
 asizeof(idigi_setting_groups)
 },
 {idigi_state_groups,
 asizeof(idigi_state_groups)
 }
};


#endif /* IDIGI_RCI_PARSER_INTERNAL_DATA */


#endif /* remote_config_h */
