#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H

#include "procnet-stdio.h"

/* Define these macros in order to intercept standard I/O,
   and pass on the data to the simulator. */
#undef printf
#define printf(...) procnet_fprintf(stdout, __VA_ARGS__)
#undef fprintf
#define fprintf procnet_fprintf
#undef putchar
#define putchar(ch) procnet_fputc((ch), stdout)
#undef putc
#define putc(ch) procnet_fputc((ch), stdout)
#undef fputc
#define fputc procnet_fputc
#undef fputs
#define fputs procnet_fputs
#undef puts
#define puts(str) procnet_fputs((str), stdout)

/* Include the project configuration. */
#ifdef PROJECT_CONF_PATH
#include PROJECT_CONF_PATH
#endif /* PROJECT_CONF_PATH */
/*---------------------------------------------------------------------------*/
#include "native-def.h"
/*---------------------------------------------------------------------------*/
#include <inttypes.h>
/*---------------------------------------------------------------------------*/

#define CC_CONF_REGISTER_ARGS          1
#define CC_CONF_FUNCTION_POINTER_ARGS  1
#define CC_CONF_VA_ARGS                1
/*#define CC_CONF_INLINE                 inline*/

#ifndef EEPROM_CONF_SIZE
#define EEPROM_CONF_SIZE				1024
#endif

typedef unsigned int uip_stats_t;

#ifndef NETSTACK_CONF_RADIO
#define NETSTACK_CONF_RADIO   procnet_radio_driver
#endif /* NETSTACK_CONF_RADIO */

#define CSMA_CONF_SEND_SOFT_ACK 1

#if NETSTACK_CONF_WITH_IPV6

#ifndef UIP_CONF_BYTE_ORDER
#define UIP_CONF_BYTE_ORDER      UIP_LITTLE_ENDIAN
#endif

/* configure network size and density */
#ifndef NETSTACK_MAX_ROUTE_ENTRIES
#define NETSTACK_MAX_ROUTE_ENTRIES   300
#endif /* NETSTACK_MAX_ROUTE_ENTRIES */
#ifndef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS 300
#endif /* NBR_TABLE_CONF_MAX_NEIGHBORS */

/* configure queues */
#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM 64
#endif /* QUEUEBUF_CONF_NUM */

#define UIP_CONF_IPV6_QUEUE_PKT  1
#define UIP_ARCH_IPCHKSUM        1

#endif /* NETSTACK_CONF_WITH_IPV6 */

#include <ctype.h>

typedef uint64_t clock_time_t;

#define CLOCK_CONF_SECOND 1000

#define LOG_CONF_ENABLED 1

#define PLATFORM_SUPPORTS_BUTTON_HAL 1

/* Not part of C99 but actually present */
int strcasecmp(const char*, const char*);

#define PLATFORM_CONF_PROVIDES_MAIN_LOOP 0
#define PLATFORM_CONF_MAIN_ACCEPTS_ARGS  1
#define PLATFORM_CONF_SUPPORTS_STACK_CHECK 0

#endif /* CONTIKI_CONF_H_ */
