#ifndef __CALCULATOR_MAIN_WIDGET_H__
#define __CALCULATOR_MAIN_WIDGET_H__

#include<gtkmm/bin.h>
#include<gtkmm/builder.h>

namespace Gtk
{
class TextView;
}


class CalculatorMainWidget: public Gtk::Bin
{
  public:
  CalculatorMainWidget();

  void set_is_voice_on(bool is);


  protected:
  Glib::RefPtr<Gtk::Builder> glade;
  Gtk::TextView* textView;
  bool voiceOn;

  void on_insert_button_clicked(std::string str);
  void on_AC_button_clicked();
  void on_cal_button_clicked();
  void on_del_button_clicked();
  void on_button_clicked_speech(std::string op);

  void on_load();
  void on_save();

  template<typename T>
  T* get_widget(std::string str)
  {
        T* weidgt;
        glade->get_widget(str, weidgt);
        return weidgt;
  }
};

std::string choose_file(void);
#endif
