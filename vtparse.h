/**
 * @file vtparse.h
 * @brief VTParse
 * @details
 * An implementation of Paul Williams' DEC compatible state machine parser
 * This code is in the public domain.
 * @author Joshua Haberman <joshua@reverberate.org>
 */

#include "vtparse_table.h"

#define MAX_INTERMEDIATE_CHARS 2
#define ACTION(state_change) (vtparse_action_t)(state_change & 0x0F)
#define STATE(state_change)  (vtparse_state_t)(state_change >> 4)

struct vtparse;

typedef void (*vtparse_callback_t)(struct vtparse*, vtparse_action_t, unsigned char);

typedef struct vtparse {
    vtparse_state_t    state;
    vtparse_callback_t cb;
    unsigned char      intermediate_chars[MAX_INTERMEDIATE_CHARS+1];
    char               ignore_flagged;
    int                params[16];
    int                num_params;
    void*              user_data;
} vtparse_t;

void vtparse_init(vtparse_t *parser, vtparse_callback_t cb);
void vtparse(vtparse_t *parser, unsigned char *data, int len);

