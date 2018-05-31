#ifndef __POKERS_H__
#define __POKERS_H__

#define POKERS_TOTAL_NUM 52
#define PLAYER_NUM 4
#define CARD_NUMTYPE_COUNT 13 //注:把牌上面的数字或字母称为numtype

typedef enum{
    CARD_FLOWER_CLUB,    //梅花
    CARD_FLOWER_DIAMOND, //方块
    CARD_FLOWER_HEART,   //红心
    CARD_FLOWER_SPADE,   //黑桃
    CARD_FLOWER_COUNT,
}card_flower;

//一张牌
typedef struct{
    int id; //标识这张牌的唯一编号
    int numType;
    card_flower flower;
    int isFront; //是否是正面
}card_t;

//一组牌
typedef struct{
    card_t *cards;
    int num;
}pokers_t;

extern char *g_card_num_str[];
extern char *g_card_flower_char[];
extern char *g_card_flower_str[];

/* -------------------------------------------------------------- */

pokers_t *pokers_new(int cardNum);       //给定该组牌的牌数,建立其储存空间
    void  pokers_del(pokers_t *pokers);       //释放由pokers_new函数分配的储存空间
     int  pokers_create(pokers_t *pokers);    //创建一副牌
    void  pokers_reshuffle(pokers_t *pokers); //洗牌
    void  pokers_deal(pokers_t *totalPokers, int  playerNum, pokers_t *container[]); //发牌
    void  pokers_sort(pokers_t *pokers,int isNumtypeFirst, int isAsc);//排序,是否优先按牌数字排序,是否升序排序

const char *card_serialize(card_t *card);          //对card序列化
    card_t  card_unserialize(const char *serStr); //反序列化解包出card
    card_t *find_card_by_id(int id, pokers_t *container[], size_t size);

      void  pokers_print(pokers_t *pokers);

/* -------------------------------------------------------------- */

enum{
ERR_CODE_ALLOC_FAIL = 1,

};

void report_error(int errCode);


#endif
