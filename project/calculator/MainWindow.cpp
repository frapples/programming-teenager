#include"MainWindow.h"


#include<gtkmm/notebook.h>
#include<gtkmm/box.h>
#include<gtkmm/toolbar.h>
#include<gtkmm/toolbutton.h>
#include<gtkmm/separatortoolitem.h>

#include<iostream>
#include<sstream>
#include<string>

#include"ToggleButton.h"
#include"CalculatorMainWidget.h"




MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade): Gtk::Window(cobject), glade(refGlade)
{
    auto toolbar = get_widget<Gtk::Toolbar>("toolbar1");


    toolbar->add(*Gtk::manage(new Gtk::SeparatorToolItem()));

    auto tabadd = Gtk::manage(new Gtk::ToolButton());
    tabadd->set_icon_name("gtk-add");
    tabadd->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_tab_add));
    toolbar->add(*tabadd);



    auto tabclose = Gtk::manage(new Gtk::ToolButton());
    tabclose->set_icon_name("gtk-close");
    tabclose->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_tab_close));
    toolbar->add(*tabclose);


    
    ToggleButton* toggle = Gtk::manage(new ToggleButton);
    get_widget<Gtk::Box>("toggle-voice-box")->add(*toggle);
    toggle->signal_toggle().connect(sigc::mem_fun(*this, &MainWindow::on_voice_toggle));

    on_tab_add();


    this->show_all_children();

}


void MainWindow::on_voice_toggle(bool state)
{
    auto notebook = get_widget<Gtk::Notebook>("notebook1");
    for (int i = 0; i < notebook->get_n_pages(); i++) {
        auto page = notebook->get_nth_page(i);
        if (page)
            ((CalculatorMainWidget*)page)->set_is_voice_on(state);
    }
}

void MainWindow::on_tab_add()
{
    auto notebook = get_widget<Gtk::Notebook>("notebook1");

    std::ostringstream o;
    o << "page";
    o << notebook->get_n_pages();

    auto tab = Gtk::manage(new CalculatorMainWidget());
    notebook->append_page(*tab, *Gtk::manage(new Gtk::Label(o.str())));




    notebook->set_show_tabs(notebook->get_n_pages() > 1);

    // 下面两句写倒了就没有卵用，不知道为什么 */
    notebook->show_all_children();
    notebook->set_current_page(notebook->get_n_pages() - 1);
}

void MainWindow::on_tab_close()
{
    auto notebook = get_widget<Gtk::Notebook>("notebook1");

    if (notebook->get_n_pages() <= 1)
        this->close();

    notebook->remove_page(notebook->get_current_page());

    notebook->set_show_tabs(notebook->get_n_pages() > 1);
}


MainWindow::~MainWindow()
{

}


