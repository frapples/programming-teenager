#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

#include<stdlib.h>
#include<string.h>
#include<assert.h>

#define ROUND_BASE 8
#define MAX_SIZE 128
#define BLOCK_COUNT 30

typedef struct stru_node{
    struct stru_node *next;
}node_t;
typedef unsigned char embedBlock_t;

void *pool_malloc(size_t size);
void *pool_malloc0(size_t size);
void  pool_free(void *p);
void  pool_free_all(void);


#endif
