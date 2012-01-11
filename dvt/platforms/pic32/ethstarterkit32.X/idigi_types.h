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

#ifndef IDIGI_TYPES_H_
#define IDIGI_TYPES_H_

#include <string.h>
#include <limits.h>
#include <assert.h>

#if __STDC_VERSION__ >= 199901L
  #include <stdint.h>
  #include <stdbool.h>
#else
  typedef unsigned char uint8_t;
  typedef unsigned short uint16_t;
  typedef unsigned long uint32_t;
  typedef int bool;
  #define true    1
  #define false   0
#endif

typedef int idigi_network_handle_t;



#if defined(__DEBUG)
#include <plib.h>
#define DEBUG_PRINTF(...)   DBPRINTF(__VA_ARGS__)
#define ASSERT(cond)        assert(cond)

#else
#define DEBUG_PRINTF(...)
#define ASSERT(cond)
#endif


#endif /* IDIGI_TYPES_H_ */
