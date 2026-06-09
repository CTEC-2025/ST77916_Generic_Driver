/*****************************************************************************
 * Module: Common definitions
 * File: DEFS.h
 * Description: Project-wide type and boolean definitions.
 * Notes: Keep target-specific type changes isolated in this file.
 *****************************************************************************/

#ifndef DEFS_H
#define DEFS_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t U8;
typedef int8_t S8;
typedef uint16_t U16;
typedef int16_t S16;
typedef uint32_t U32;
typedef int32_t S32;

#define FALSE                         (0u)
#define TRUE                          (!FALSE)

#endif
