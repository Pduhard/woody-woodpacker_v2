#ifndef BLOWFISH_H
# define BLOWFISH_H

# include <math.h>

// # define BLOWFISH_ORIGINAL_DATA_FILE    ""

typedef struct s_blowfish_keys
{
    uint32_t    P[18];
    uint32_t    S[4][256];
}               t_blowfish_keys;

#endif