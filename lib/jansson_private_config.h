/**
* Here, we provide a configuration for Jansson.
*/

#include <inttypes.h>
#include <sys/types.h>

#define JSON_PARSER_MAX_DEPTH 2048



#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H

#define JSON_INLINE inline
#define JSON_HAVE_LOCALECONV 0

#define JSON_INTEGER_IS_LONG_LONG 0
#define HAVE_UNISTD_H
#define HAVE_SYS_TYPES_H
#define HAVE_INTTYPES_H
#define HAVE_CONFIG_H

#endif
