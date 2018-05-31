#ifndef __GTK_GUI__H__
#define __GTK_GUI__H__

#include"pokers.h"

/* -------------------------------------------------------------- */

GtkWidget *get_root_layout(pokers_t *playerPokers[]);   //绘制扑克牌显示主界面
GtkWidget *get_column_player_layout(pokers_t *pokers, int pos, GtkWidget *label); //绘制左右两列的玩家布局
GtkWidget *get_line_player_layout(pokers_t *pokers);    //绘制上下两行的玩家布局
GtkWidget *get_center_image_layout(void);               //绘制中心大图片
GtkWidget *get_center_toggle_button_layout(void);       //绘制中心按钮区
     void  flush_root_layout(GtkWidget *window);              //刷新扑克牌显示主界面

/* -------------------------------------------------------------- */

  void  config_set_sort_type(int isNumtypeFirst, int isAsc);
  void  config_set_center_card(card_t *card);
card_t *config_get_center_card(void);

/* -------------------------------------------------------------- */

/*选择对应card的图片,返回按钮控件或图片控件,mul可控制图片放大倍数
 *此函数会将card的id属性对返回的控件设置名称 */
GtkWidget *get_pokers_image(card_t *card, float mul, int isReturnPicWidgt);
GtkWidget *get_image_widget_from_path(const char *path, float mul); //从对应path路径返回image控件
     void  gtk_toggle_button_image_load(GtkWidget *button, int isOn);     //设置开关类按钮的图片
     void  load_button_image(GtkWidget *button);          //对开关类按钮,切换其图片(如现在是开则改为关)

#define MUSIC_CONTROL_ON  1
#define MUSIC_CONTROL_OFF 0
void music_control(int control); //音乐控制

/* -------------------------------------------------------------- */
/* G_CALLBACK回调 */
void callback_reshuffle(GtkWidget *widget, gpointer);
void callback_poker_knocked(GtkWidget *button, gpointer);
void callback_sort_mode_toggle(GtkWidget *button, gpointer);
void callback_front_all_card(GtkWidget *button, gpointer);
void callback_music_toggle(GtkWidget *button, gpointer);
void main_quit(void);

/* -------------------------------------------------------------- */

void fill_new_pokers(pokers_t *container[]);

#endif
