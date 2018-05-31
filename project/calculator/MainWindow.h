#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include<gtkmm/window.h>
#include<gtkmm/builder.h>



class MainWindow: public Gtk::Window
{
public:
MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
virtual ~MainWindow();



void on_tab_add();
void on_tab_close();
void on_voice_toggle(bool status);




protected:
Glib::RefPtr<Gtk::Builder> glade;

template<typename T>
T* get_widget(std::string str)
{
      T* weidgt;
      glade->get_widget(str, weidgt);
      return weidgt;
}

};




#endif
