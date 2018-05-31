#include<gtk/gtk.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include"GTK_GUI.h"

struct {
    pokers_t *playerPokers[PLAYER_NUM];
    //card_t *centerCard; 注：排序时算法交换的card结构体的值，记录指针的话不能确保所指的位置是同一张牌
    int centerCardId;
    int isNumtypeFirst;
    int isAsc;
    int isMusicPlay;
}g_ui_config;

int main(void)
{
    gtk_init(NULL, NULL);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "扑克牌洗牌");
    g_signal_connect(window, "destroy", G_CALLBACK(main_quit), NULL);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    fill_new_pokers(g_ui_config.playerPokers);
    config_set_sort_type(TRUE, TRUE);
    config_set_center_card(NULL);

    g_ui_config.isMusicPlay = FALSE;
    //music_control(MUSIC_CONTROL_ON);
    
    flush_root_layout(window);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}


GtkWidget *get_root_layout(pokers_t *playerPokers[])
{
    GtkWidget *root = gtk_grid_new();

    /* 上方玩家 */
	GtkWidget *topBox = get_line_player_layout(playerPokers[0]);
    gtk_grid_attach(GTK_GRID(root), topBox, 0, 0, 13, 1);
    GtkWidget *topLabel = gtk_label_new("玩家1\n");
    gtk_grid_attach(GTK_GRID(root), topLabel, 0, 1, 13, 1);

    /* 左方玩家 */
    GtkWidget *leftLabel = gtk_label_new("玩家2   ");
    GtkWidget *leftBox = get_column_player_layout(playerPokers[1], GTK_POS_LEFT, leftLabel);
    gtk_grid_attach(GTK_GRID(root), leftBox, 0, 2, 4, 5);

    /** 中间区域 **/
    GtkWidget *centerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_grid_attach(GTK_GRID(root), centerBox, 4, 2, 5, 5);

    GtkWidget *centerTopLabel = gtk_label_new("\n");
    gtk_container_add(GTK_CONTAINER(centerBox), centerTopLabel);

   
    GtkWidget *imgBox = get_center_image_layout();
    gtk_container_add(GTK_CONTAINER(centerBox), imgBox);

    GtkWidget *centerBottonLabel = gtk_label_new("\n");
    gtk_container_add(GTK_CONTAINER(centerBox), centerBottonLabel);

    GtkWidget *featureBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(centerBox), featureBox);

    GtkWidget *alignLabel = gtk_label_new("\t\t");
    gtk_container_add(GTK_CONTAINER(featureBox), alignLabel);
    GtkWidget *reshuffleButton = gtk_button_new_with_label("重新洗牌");
    gtk_container_add(GTK_CONTAINER(featureBox), reshuffleButton);
    g_signal_connect(reshuffleButton, "clicked", G_CALLBACK(callback_reshuffle), NULL);

    GtkWidget *onekeyButton = gtk_button_new_with_label("一键敲开");
    gtk_container_add(GTK_CONTAINER(featureBox), onekeyButton);
    g_signal_connect(onekeyButton, "clicked", G_CALLBACK(callback_front_all_card), NULL);


    GtkWidget *toggleBox = get_center_toggle_button_layout();
    gtk_container_add(GTK_CONTAINER(centerBox), toggleBox);
    /** ------ **/

    /* 右方玩家 */
    GtkWidget *rightLabel = gtk_label_new("   玩家3");
    GtkWidget *rightBox = get_column_player_layout(playerPokers[2], GTK_POS_RIGHT, rightLabel);
    gtk_grid_attach(GTK_GRID(root), rightBox, 9, 2, 4, 5);

    /* 下方玩家 */
    GtkWidget *bottomLabel = gtk_label_new("\n玩家4");
    gtk_grid_attach(GTK_GRID(root), bottomLabel, 0, 7, 13, 1);
	GtkWidget *bottomBox = get_line_player_layout(playerPokers[3]);
    gtk_grid_attach(GTK_GRID(root), bottomBox, 0, 8, 13, 1);

    return root;
}

GtkWidget *get_center_image_layout(void)
{
    GtkWidget *imgBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    
    gtk_container_add(GTK_CONTAINER(imgBox), get_pokers_image(config_get_center_card(), 2, TRUE));
    return imgBox;
}

GtkWidget *get_center_toggle_button_layout(void)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    
    GtkWidget *musicBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(box), musicBox);
    GtkWidget *musicLabel = gtk_label_new("\t\t背景音乐:  ");
    gtk_container_add(GTK_CONTAINER(musicBox), musicLabel);
    GtkWidget *musicButton = gtk_button_new();
    g_signal_connect(musicButton, "clicked", G_CALLBACK(callback_music_toggle), NULL);
    gtk_widget_set_name(musicButton, "music");
    load_button_image(musicButton);
    gtk_container_add(GTK_CONTAINER(musicBox), musicButton);

    GtkWidget *numtypeFirstBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(box), numtypeFirstBox);
    GtkWidget *numtypeFirstLabel = gtk_label_new("\t 数字优先排序:  ");
    gtk_container_add(GTK_CONTAINER(numtypeFirstBox), numtypeFirstLabel);
    GtkWidget *numtypeFirstButton = gtk_button_new();
    g_signal_connect(numtypeFirstButton, "clicked", G_CALLBACK(callback_sort_mode_toggle), NULL);
    gtk_container_add(GTK_CONTAINER(numtypeFirstBox), numtypeFirstButton);
    gtk_widget_set_name(numtypeFirstButton, "numtype");
    load_button_image(numtypeFirstButton);

    GtkWidget *ascBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(box), ascBox);
    GtkWidget *ascLabel = gtk_label_new("\t 从小到大排序:  ");
    gtk_container_add(GTK_CONTAINER(ascBox), ascLabel);
    GtkWidget *ascButton = gtk_button_new();
    g_signal_connect(ascButton, "clicked", G_CALLBACK(callback_sort_mode_toggle), NULL);
    gtk_container_add(GTK_CONTAINER(ascBox), ascButton);
    gtk_widget_set_name(ascButton, "asc");
    load_button_image(ascButton);

    return box;
}

GtkWidget *get_column_player_layout(pokers_t *pokers, int pos, GtkWidget *label)
{
    int numPerCol[3];
    GtkWidget  *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    switch(pos){
        case GTK_POS_LEFT:
            numPerCol[0] 
                = numPerCol[1]
                = pokers->num /3 + 1;
            numPerCol[3] = pokers->num - 2 * (pokers->num / 3 + 1);

        break;

        case GTK_POS_RIGHT:
            numPerCol[1] 
                = numPerCol[2]
                = pokers->num /3 + 1;
            numPerCol[0] = pokers->num - 2 * (pokers->num / 3 + 1);

            gtk_container_add(GTK_CONTAINER(box), label);
    }

    int col, i = 0;
    for(col = 0; col < 3; col++) {
        GtkWidget *subBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); 
        gtk_container_add(GTK_CONTAINER(box), subBox);
        int n;
        for(n = 0; n < numPerCol[col] && i < pokers->num; n++,i++){
            GtkWidget *picButton = get_pokers_image(&(pokers->cards[i]), 1, FALSE);
            gtk_container_add(GTK_CONTAINER(subBox), picButton);
            g_signal_connect(picButton, "clicked", G_CALLBACK(callback_poker_knocked), NULL);
        }
    }

    if(pos == GTK_POS_LEFT)
        gtk_container_add(GTK_CONTAINER(box), label);
    return box;
}

GtkWidget *get_line_player_layout(pokers_t *pokers)
{
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    int i;
    for(i = 0; i < pokers->num; i++) {
            GtkWidget *picButton = get_pokers_image(&(pokers->cards[i]), 1, FALSE);
            gtk_container_add(GTK_CONTAINER(box), picButton);
            g_signal_connect(picButton, "clicked", G_CALLBACK(callback_poker_knocked), NULL);
    }
    return box;
}

void flush_root_layout(GtkWidget *window)
{
    static GtkWidget *rootLayout = NULL;

    assert(window || rootLayout);

    if(!window)
        window = gtk_widget_get_parent(rootLayout);

    if(rootLayout)
        gtk_widget_destroy(rootLayout);

    rootLayout = get_root_layout(g_ui_config.playerPokers);
    gtk_container_add(GTK_CONTAINER(window), rootLayout);
    gtk_widget_show_all(rootLayout);
}

/* -------------------------------------------------------------- */

void config_set_sort_type(int isNumtypeFirst, int isAsc)
{
    g_ui_config.isNumtypeFirst = isNumtypeFirst;
    g_ui_config.isAsc          = isAsc;

    int i;
    for(i = 0; i < PLAYER_NUM; i++) {
        pokers_sort(g_ui_config.playerPokers[i],
                g_ui_config.isNumtypeFirst, g_ui_config.isAsc);
    }
}

void config_set_center_card(card_t *card)
{
    if(card)
        g_ui_config.centerCardId = card->id;
}

card_t *config_get_center_card(void)
{
    return find_card_by_id(g_ui_config.centerCardId, g_ui_config.playerPokers, PLAYER_NUM);
}

/* -------------------------------------------------------------- */

GtkWidget *get_pokers_image(card_t *card, float mul, int isReturnPicWidgt)
{
    char path[FILENAME_MAX];
    if(!card)
        sprintf(path, "pic/R_K.jpg");
    else if(card->isFront)
        sprintf(path, "pic/%d_%d.jpg", card->numType + 1, card->flower + 1);
    else
        sprintf(path, "pic/back.jpg");


    GtkWidget *image = get_image_widget_from_path(path, mul);

    if(!isReturnPicWidgt){
        GtkWidget *button = gtk_button_new();
        gtk_button_set_image(GTK_BUTTON(button), image);
        GString *buf = g_string_new(NULL);
        g_string_printf(buf, "%d", card->id);
        gtk_widget_set_name(button, buf->str);
        return button;
    }
    return image;
}

GtkWidget *get_image_widget_from_path(const char *path, float mul)
{
    GdkPixbuf *src = gdk_pixbuf_new_from_file(path, NULL);	
	GdkPixbuf *dst = gdk_pixbuf_scale_simple(src, 105 * 2.5 / 5.0 * mul, 150 * 2.5 / 5.0 *mul, GDK_INTERP_BILINEAR);
    GtkWidget *image = gtk_image_new_from_pixbuf(dst);
	g_object_unref(src);
	g_object_unref(dst);
    return image;
}

void gtk_toggle_button_image_load(GtkWidget *button, int isOn)
{
    char *path;
    if(isOn)
        path = "./pic/toggle-on.svg";
    else
        path = "./pic/toggle-off.svg";

    GtkWidget *img = gtk_image_new_from_file(path);
    gtk_button_set_image(GTK_BUTTON(button), img);
}

void load_button_image(GtkWidget *button)
{
    if(strcmp(gtk_widget_get_name(button), "numtype") == 0)
        gtk_toggle_button_image_load(button, g_ui_config.isNumtypeFirst);
    else if(strcmp(gtk_widget_get_name(button), "asc") == 0)
        gtk_toggle_button_image_load(button, g_ui_config.isAsc);
    else
        gtk_toggle_button_image_load(button, g_ui_config.isMusicPlay);
}

void music_control(int control)
{
    static FILE *musicPopen = NULL;
   if(control == MUSIC_CONTROL_ON) {
        if(musicPopen) {
            fprintf(musicPopen, "q");
            pclose(musicPopen);
        } else
            musicPopen = popen("mplayer test.mp3 > /dev/null", "w");
    } else {
        if(musicPopen){
            fprintf(musicPopen, "q");
            pclose(musicPopen);
            musicPopen = NULL;
        }
    }
}

/* -------------------------------------------------------------- */


void callback_reshuffle(GtkWidget *widget, gpointer nothing)
{
    fill_new_pokers(g_ui_config.playerPokers);
    config_set_center_card(NULL);
    flush_root_layout(NULL);
}

void callback_poker_knocked(GtkWidget *button, gpointer nothing)
{
    int id = atoi(gtk_widget_get_name(button));

    card_t *pCard = find_card_by_id(id, g_ui_config.playerPokers, PLAYER_NUM);
    pCard->isFront = TRUE;

    config_set_center_card(pCard);

    flush_root_layout(NULL);
}

void callback_music_toggle(GtkWidget *button, gpointer data)
{
    g_ui_config.isMusicPlay = !(g_ui_config.isMusicPlay);
    if(g_ui_config.isMusicPlay)
        music_control(MUSIC_CONTROL_ON);
    else
        music_control(MUSIC_CONTROL_OFF);

    load_button_image(button);
}

void callback_sort_mode_toggle(GtkWidget *button, gpointer nothing)
{
    int isNumtypeFirst = g_ui_config.isNumtypeFirst;
    int isAsc          = g_ui_config.isAsc;
    if(strcmp(gtk_widget_get_name(button), "numtype") == 0)
        isNumtypeFirst = !(g_ui_config.isNumtypeFirst);
    else
        isAsc = !(g_ui_config.isAsc);

    config_set_sort_type(isNumtypeFirst, isAsc);

    flush_root_layout(NULL);
}

void callback_front_all_card(GtkWidget *button, gpointer nothing)
{
    int i;
    for(i = 0; i < PLAYER_NUM; i++){
        int j;
        for(j = 0; j < g_ui_config.playerPokers[i]->num; j++)
            g_ui_config.playerPokers[i]->cards[j].isFront = TRUE;
    }

    flush_root_layout(NULL);
}

void main_quit(void)
{
    music_control(MUSIC_CONTROL_OFF);
    gtk_main_quit();
}

/* -------------------------------------------------------------- */

void fill_new_pokers(pokers_t *container[])
{
    /* 扑克牌数据准备 */
    pokers_t *pokers = pokers_new(POKERS_TOTAL_NUM);
    pokers_create(pokers);   //拿一副整齐的扑克牌
    pokers_reshuffle(pokers);//洗牌
    pokers_deal(pokers, PLAYER_NUM, container); //模拟人工的方法按顺序发给每个人
    pokers_del(pokers);
}
