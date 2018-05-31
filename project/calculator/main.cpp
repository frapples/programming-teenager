#include<gtkmm/application.h>

#include<iostream>
#include<gtkmm/builder.h>
#include<gtkmm/cssprovider.h>


#include"MainWindow.h"



int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv);

    auto builder = Gtk::Builder::create_from_file("ui.glade", "window");


    auto css = Gtk::CssProvider::create();
    css->load_from_path("style.css");

    MainWindow* win = 0;
    builder->get_widget_derived("window", win);


    Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


    app->run(*win);


    return 0;
}

