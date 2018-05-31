/*
编写一个模拟人工洗牌的程序，将洗好的牌分别发给四个人。
使用结构card 来描述一张牌，用随机函数来模拟人工洗牌的过程，最后将洗好的52张牌顺序分别发给四个人。
对每个人的牌要按桥牌的规则输出。
即一个人的牌要先按牌的花色（顺序为梅花、方块、红心和黑桃）进行分类，
同一类的牌要再按A、K、Q、J、…、3、2牌的大小顺序排列。
另发牌应按四个人的顺序依次分发。
*/

#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include"pokers.h"

char *g_card_num_str[] = { \
    "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" \
};

char *g_card_flower_char[] = {"♣ ", "♦ ", "♥ ", "♠ "};


char *g_card_flower_str[] = { \
    [CARD_FLOWER_CLUB] = "梅花", \
    [CARD_FLOWER_DIAMOND] = "方块", \
    [CARD_FLOWER_HEART] = "红心", \
    [CARD_FLOWER_SPADE] = "黑桃" \
};

/* =============================================== */

//供qsort函数排序使用的回调函数
static int cmp_card_by_flower(const void *card1, const void *card2);
static int cmp_card_by_numtype(const void *card1, const void *card2);
static int cmp_card_by_numtype_desc(const void *card1, const void *card2);

static void pokers_sort_numtpye_first(pokers_t *pokers, int isAsc);
static void pokers_sort_flower_first(pokers_t *pokers, int isAsc);
/*
int main(void)
{
    pokers_t *pokers = pokers_new(POKERS_TOTAL_NUM);
    pokers_create(pokers);
    puts("洗牌前");
    pokers_print(pokers);

    pokers_reshuffle(pokers);
    puts("洗牌后:");
    pokers_print(pokers);

    pokers_t *subPokers[PLAYER_NUM];
    pokers_deal(pokers, PLAYER_NUM, subPokers);

    int i;
    for(i = 0; i < PLAYER_NUM; i++){
        printf("\nPlayer %d:\n", i);
        pokers_sort(subPokers[i]);
        pokers_print(subPokers[i]);
    }

    UI_draw_card(NULL, subPokers[i]->cards[i]);

    for(i = 0; i < PLAYER_NUM; i++)
        pokers_del(subPokers[i]);
    pokers_del(pokers);
    return 0;
}
*/


pokers_t *pokers_new(int cardNum)
{
    pokers_t *pokers = malloc(sizeof(pokers_t));
    if(!pokers)
        report_error(ERR_CODE_ALLOC_FAIL);
    pokers->num = cardNum;
    pokers->cards = calloc(cardNum, sizeof(card_t));
    if(!pokers->cards)
        report_error(ERR_CODE_ALLOC_FAIL);
    return pokers;
}

void pokers_del(pokers_t *pokers)
{
    free(pokers->cards);
    free(pokers);
}

//填充一副有序的扑克牌
int pokers_create(pokers_t *pokers)
{
    card_t *card = pokers->cards;
    int i = 0;
    int n, flr;
     for(n = 0; n < CARD_NUMTYPE_COUNT; n++) //数字类型
        for(flr = 0; flr < CARD_FLOWER_COUNT; flr++){ //花色类型
            card->numType = n;
            card->flower = flr;
            card->id = i;
            card++, i++;
        }
   return 0;
}

//目前为调试所用，输出poker中的牌
void pokers_print(pokers_t *pokers)
{
    int i;
    card_t *card = pokers->cards;
    for(i = 0; i < pokers->num; i++,card++)
        printf("[card%2d:(%s%2s)]\n", i, g_card_flower_str[card->flower], 
                                         g_card_num_str[card->numType]);
}

//洗牌
void pokers_reshuffle(pokers_t *pokers)
{
    static int sranded = 0;
    if(!sranded){
        srand(time(NULL));
        sranded = 1;
    }
    int i;
    for(i = 0; i < pokers->num; i++){
        int randNum = rand() % pokers->num;
        if(i != randNum){ //交换
            card_t tmp = pokers->cards[i];
            pokers->cards[i] = pokers->cards[randNum];
            pokers->cards[randNum] = tmp;
        }
    }
}

void pokers_deal(pokers_t *totalPokers, int playerNum, pokers_t *container[])
{
    int subNum = totalPokers->num / playerNum;

    //指针数组先置0,后面要做NULL标记
    memset(container, 0, sizeof(pokers_t *) * playerNum);
    int i;
    for(i = 0; i < totalPokers->num; i++){
        int id = i % playerNum;
        if(!container[id]) //发的是第一张牌的话,先建立数据储存空间
            container[id] = pokers_new(subNum);
        container[id]->cards[i / playerNum] = totalPokers->cards[i];
    }
}

void pokers_sort(pokers_t *pokers, int isNumtypeFirst, int isAsc)
{
    if(isNumtypeFirst)
        pokers_sort_numtpye_first(pokers, isAsc);
    else
        pokers_sort_flower_first(pokers, isAsc);
}

typedef int (*QsortCmp_t)(const void *, const void *);
static void pokers_sort_flower_first(pokers_t *pokers, int isAsc)
{

    QsortCmp_t cmpNumtypeCallback = isAsc ? cmp_card_by_numtype : cmp_card_by_numtype_desc;
    //先按花色排序
    qsort(pokers->cards, pokers->num, sizeof(card_t), cmp_card_by_flower);

    //flrNode的元素指向同花色的牌组的分界处,指向下一组牌的第一张
    card_t *flrNode[CARD_FLOWER_COUNT + 1], **node = flrNode;
    *node = pokers->cards;
    card_t *card;
    int i;
    for(i = 0,card = pokers->cards; i < pokers->num; i++,card++)
        if((*node)->flower != card->flower)
            *(++node) = card;
    *(++node) = pokers->cards + pokers->num;

    //对每一组花色牌,再按数字类型排序
    for(i = 0; i < node - flrNode; i++) //注:条件若写i < CARD_FLOWER_COUNT, 有大坑bug
        qsort(flrNode[i], flrNode[i + 1] - flrNode[i], sizeof(card_t), cmpNumtypeCallback);
}

static void pokers_sort_numtpye_first(pokers_t *pokers, int isAsc)
{
    QsortCmp_t cmpNumtypeCallback = isAsc ? cmp_card_by_numtype : cmp_card_by_numtype_desc;
    //先按数字类型排序
    qsort(pokers->cards, pokers->num, sizeof(card_t), cmpNumtypeCallback);

    //node的元素指向不同数字牌的分界处,指向下一组牌的第一张
    card_t *nodes[CARD_NUMTYPE_COUNT + 1], **node = nodes;
    *node = pokers->cards;
    card_t *card;
    int i;
    for(i = 0,card = pokers->cards; i < pokers->num; i++,card++)
        if((*node)->numType != card->numType)
            *(++node) = card;
    *(++node) = pokers->cards + pokers->num;

    //再按花色排序
    for(i = 0; i < node - nodes; i++)
        qsort(nodes[i], nodes[i + 1] - nodes[i], sizeof(card_t), cmp_card_by_flower);
}


static int cmp_card_by_flower(const void *card1, const void *card2)
{
    return ((card_t *)card1)->flower - ((card_t *)card2)->flower;
}

static int cmp_card_by_numtype_desc(const void *card1, const void *card2)
{
    return cmp_card_by_numtype(card2, card1);
}

static int cmp_card_by_numtype(const void *card1, const void *card2)
{
    int numType1 = ((card_t *)card1)->numType;
    int numType2 = ((card_t *)card2)->numType;

    if(numType1 == 0) return 1;
    if(numType2 == 0) return -1;
    return numType1 - numType2;
}

/* ------------------------------------------------- */

#define SERIALIZE_MAX_LEN 20
const char *card_serialize(card_t *card)
{
    static char serStr[SERIALIZE_MAX_LEN];
    sprintf(serStr, "%d-%d-%d-%d", card->id, card->numType, card->flower, card->isFront);
    return serStr;
}

card_t card_unserialize(const char *serStr)
{
    card_t card;
    sscanf(serStr, "%d-%d-%d-%d", &(card.id), &(card.numType), (int *)&(card.flower), &(card.isFront));
    return card;
}

card_t *find_card_by_id(int id, pokers_t *container[], size_t size)
{
    int i, j;
    
    for(i = 0; i < size; i++)
        for(j = 0; j < container[i]->num; j++)
            if(container[i]->cards[j].id == id)
                return &(container[i]->cards[j]);
    return NULL;
}


void report_error(int errCode)
{
    switch(errCode){
        case ERR_CODE_ALLOC_FAIL:
            fputs("程序执行错误:内存申请失败!", stderr);
            exit(errCode);
    }
}
