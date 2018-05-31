#include"mempool.h"

static node_t *g_freelist[MAX_SIZE / ROUND_BASE] = {NULL};
static node_t *g_malloclist = NULL; //向系统申请的空间的链表，嵌在每次向系统申请空间的头部


static  size_t round_up(size_t size);
static  size_t get_freelist_index(size_t size);
static  size_t get_block_size_by_node(node_t **pNode);
static    void fill_freelist(node_t **pNode, void *p, size_t size);
static    void node_add(node_t **pNode, node_t *newNode);
static node_t *node_pop(node_t **pNode);


void *pool_malloc(size_t size)
{
    assert(size <= MAX_SIZE && "超过了内存池一次申请的最大字节");
    size = round_up(size);
    size_t index = get_freelist_index(size);
    node_t **pNode = &g_freelist[index];

    assert(index >= 0);
    assert(index < sizeof(g_freelist) / sizeof(*g_freelist));

    if(!*pNode) { //空间不够
        size_t blocksSize = get_block_size_by_node(pNode) * BLOCK_COUNT;
        void *p = malloc(blocksSize + sizeof(node_t));
        node_add(&g_malloclist, (node_t *)p);
        p = (node_t *)p + 1;
        if(!p)
            return NULL;

        fill_freelist(pNode, p, blocksSize);
    }

    embedBlock_t *p = (embedBlock_t *)*pNode;
    node_pop(pNode); //和下一句不可颠倒！下一句赋值会覆盖掉链表信息！
    *p = (embedBlock_t)index;
    return p + 1;
}


void *pool_malloc0(size_t size)
{
    void *p = pool_malloc(size);
    if(p)
        memset(p, 0, size);
    return p;
}

void pool_free(void *p)
{
    assert(p != NULL);

    p = (embedBlock_t *)p - 1;
    size_t index = *(embedBlock_t *)p;

    assert(index >= 0);
    assert(index < sizeof(g_freelist) / sizeof(*g_freelist));

    node_t **pNode = &g_freelist[index];
    node_add(pNode, (node_t *)p);
}

void pool_free_all(void)
{
    node_t *node;
    while((node = node_pop(&g_malloclist)))
        free(node);
}

static size_t round_up(size_t size)
{
    if(size % ROUND_BASE)
        return (size / ROUND_BASE + 1) * ROUND_BASE;
    else
        return size;
    //return (size + 7) & ~7;
}

static size_t get_freelist_index(size_t size)
{
    return round_up(size) / ROUND_BASE - 1;
}

static size_t get_block_size_by_node(node_t **pNode)
{
    assert(pNode >= g_freelist);
    return sizeof(embedBlock_t) + ((pNode - g_freelist + 1) * ROUND_BASE);
}



static void fill_freelist(node_t **pNode, void *p, size_t size)
{
    int block_size = get_block_size_by_node(pNode);
    int n = size / block_size;
    while(n-- > 0) {
        node_add(pNode, (node_t *)p);
        p = (char *)p + block_size;
    }
}

static void node_add(node_t **pNode, node_t *newNode)
{
    newNode->next = *pNode;
    *pNode = newNode;
}

static node_t *node_pop(node_t **pNode)
{
    if(*pNode) {
        node_t *node = *pNode;
        *pNode = (*pNode)->next;
        return node;
    }
    return NULL;
}
