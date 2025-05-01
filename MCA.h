#ifndef MCA_H
#define MCA_H

#include <stdint.h>

typedef struct tag_MCO_t
{
    uint32_t wl : 7;
    uint32_t str : 2;
    uint32_t bank : 2;
    uint32_t page : 2;
    uint32_t channel : 3;
    uint32_t plane : 1;
    uint32_t cluster_offset : 2;
} MCO_t;

typedef struct tag_MCA_T
{
    uint32_t MBA;
    MCO_t mco;
} MCA_t;

#endif
