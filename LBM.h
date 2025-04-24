#ifndef LBM_H
#define LBM_H

#include <stdint.h>

// Constants
#define BAD_BLOCK_THRESHOLD_PERCENT 30 // Retire if bad_block_count > 30%

// You must define these in your build or another included header


// LogicalBlock structure
typedef struct tag_LogicalBlock_t {
    uint32_t mba;
    uint32_t bad_block_count;
    uint32_t vccount;
    uint32_t physical_block_id;
    uint32_t wl;
    uint16_t str;
} LogicalBlock_t;

// Node for the doubly linked list
typedef struct tag_LogicalBlockNode_t {
    LogicalBlock_t block;
    struct tag_LogicalBlockNode_t *prev;
    struct tag_LogicalBlockNode_t *next;
} LogicalBlockNode_t;

// List structure
typedef struct tag_LogicalBlockList_t {
    LogicalBlockNode_t *head;
    LogicalBlockNode_t *tail;
} LogicalBlockList_t;

// Global lists and input block
extern LogicalBlockList_t free_list;
extern LogicalBlockList_t active_list;
extern LogicalBlockList_t bad_block_list;
extern LogicalBlock_t input_list;

// Function declarations
void lbm_add_to_tail(LogicalBlockList_t *list, LogicalBlock_t block);
void LBMinit(void);
void print_logical_block_list(const char *name, LogicalBlockList_t *list);
LogicalBlock_t lbm_allocate_block(void);
void lbm_move_input_to_active(LogicalBlock_t block);
void lbm_periodic_vcc_check(void);
void lbm_retire_logical_block(void);

#endif // LBM_H
