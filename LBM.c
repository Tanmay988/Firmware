#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "FE.h"
#include "LBM.h"

// Declare two lists: free_list and active_list
LogicalBlockList_t free_list = {NULL, NULL};
LogicalBlockList_t active_list = {NULL, NULL};
LogicalBlockList_t bad_block_list = {NULL, NULL};

LogicalBlock_t input_list;

void lbm_add_to_tail(LogicalBlockList_t *list, LogicalBlock_t block)
{
    LogicalBlockNode_t *new_node = (LogicalBlockNode_t *)malloc(sizeof(LogicalBlockNode_t));
    new_node->block = block;
    new_node->next = NULL;
    new_node->prev = list->tail;

    if (list->tail)
        list->tail->next = new_node;
    else
        list->head = new_node;

    list->tail = new_node;
}

// LBMinit implementation
void LBMinit()
{
    for (uint32_t i = 0; i < TOTAL_SUPERBLOCKS; i++)
    {
        LogicalBlock_t block = {0};
        block.mba = i;
        lbm_add_to_tail(&free_list, block);
    }
    printf("LBM: Initialized %d logical blocks in free_list\n", TOTAL_SUPERBLOCKS);
}

// Function to print the logical block list
void print_logical_block_list(const char *name, LogicalBlockList_t *list)
{
    printf("\n--- %s ---\n", name);
    LogicalBlockNode_t *current = list->head;
    int index = 0;

    while (current)
    {
        printf("Index %d: MBA: %u, Bad Count: %u, VCCount: %u, PBlock ID: %u, WL: %u, STR: %u\n",
               index++,
               current->block.mba,
               current->block.bad_block_count,
               current->block.vccount,
               current->block.physical_block_id,
               current->block.wl,
               current->block.str);
        current = current->next;
    }

    if (index == 0)
        printf("List is empty!\n");
}

LogicalBlock_t lbm_allocate_block()
{
    if (free_list.head == NULL)
    {
        printf("LBM Error: Free list is empty. Cannot allocate block.\n");
        while (1)
        {
        };
    }

    LogicalBlockNode_t *allocated_node = free_list.head;

    // Update the free_list to remove the node
    free_list.head = allocated_node->next;
    if (free_list.head)
        free_list.head->prev = NULL;
    else
        free_list.tail = NULL; // list became empty

    LogicalBlock_t block = allocated_node->block;
    free(allocated_node); // free node memory since we only return block

    printf("LBM: Allocated block with MBA = %u\n", block.mba);
    return block;
}

// Function to move a logical block from input_list to active list
void lbm_move_input_to_active(LogicalBlock_t block)
{
    // Allocate a new node and add to tail of active_list
    LogicalBlockNode_t *new_node = (LogicalBlockNode_t *)malloc(sizeof(LogicalBlockNode_t));
    if (!new_node)
    {
        printf("LBM Error: Memory allocation failed while moving block to active list.\n");
        return;
    }

    new_node->block = block;
    new_node->prev = active_list.tail;
    new_node->next = NULL;

    if (active_list.tail)
        active_list.tail->next = new_node;
    else
        active_list.head = new_node;

    active_list.tail = new_node;

    printf("LBM: Moved block with MBA = %u to active list.\n", block.mba);
}

// Function vcc check for active list
void lbm_periodic_vcc_check()
{
    LogicalBlockNode_t *current = active_list.head;

    while (current != NULL)
    {
        LogicalBlockNode_t *next_node = current->next;

        if (current->block.vccount == 0)
        {
            printf("LBM: Moving block MBA = %u from active_list to free_list (vccount == 0)\n", current->block.mba);

            // Save block data
            LogicalBlock_t block = current->block;

            // Remove current node from active_list
            if (current->prev)
                current->prev->next = current->next;
            else
                active_list.head = current->next;

            if (current->next)
                current->next->prev = current->prev;
            else
                active_list.tail = current->prev;

            // Free the current node memory
            free(current);

            // Add the block back to the free_list using lbm_add_to_tail
            lbm_add_to_tail(&free_list, block);
        }

        current = next_node;
    }
}

// function is used to check count of bad blocks in active list if it is greater than threshold then retire the block
void lbm_retire_logical_block()
{
    LogicalBlockNode_t *current = active_list.head;

    while (current != NULL)
    {
        LogicalBlockNode_t *next_node = current->next;

        // Let's assume threshold is calculated per block for simplicity
        // You may adjust this based on actual physical block range
        uint32_t threshold = (TOTAL_PHYSICALbLOCK_PER_SUPERBLOCK * BAD_BLOCK_THRESHOLD_PERCENT) / 100;

        if (current->block.bad_block_count > threshold)
        {
            printf("LBM: Retiring block MBA = %u due to bad_block_count = %u\n",
                   current->block.mba, current->block.bad_block_count);

            LogicalBlock_t block = current->block;

            // Remove from active list
            if (current->prev)
                current->prev->next = current->next;
            else
                active_list.head = current->next;

            if (current->next)
                current->next->prev = current->prev;
            else
                active_list.tail = current->prev;

            free(current);

            // Add to bad_block_list
            lbm_add_to_tail(&bad_block_list, block);
        }

        current = next_node;
    }
}

int main()
{
    // Initialize the LBM
    LBMinit();

    // Print the free list
    print_logical_block_list("Free List", &free_list);

    // Print the active list (should be empty at this point)
    print_logical_block_list("Active List", &active_list);

    return 0;
}