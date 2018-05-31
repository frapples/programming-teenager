#ifndef __SWITCH_BUTTON_H__
#define __SWITCH_BUTTON_H__

#include<gtkmm/button.h>
#include<gtkmm/image.h>

class ToggleButton: public Gtk::Button
{
    public:
    ToggleButton(): state(true), onImage("png/toggle-on.svg"), offImage("png/toggle-off.svg")
    {
        this->set_image(state ? onImage : offImage);
        this->set_always_show_image();
        this->signal_clicked().connect( sigc::mem_fun(*this, &ToggleButton::on_button_clicked) );
    }

    virtual ~ToggleButton() { }
    sigc::signal<void, bool> signal_toggle()
    {
        return signalToggle;
    }

    void on_button_clicked()
    {
        state = !state;
        this->set_image(state ? onImage : offImage);
        signal_toggle().emit(state);
    }


    
    private:
    bool state;
    Gtk::Image onImage;
    Gtk::Image offImage;

    sigc::signal<void, bool> signalToggle;

};

#endif
