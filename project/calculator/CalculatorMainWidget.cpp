#include"CalculatorMainWidget.h"

#include<gtkmm/grid.h>
#include<gtkmm/button.h>
#include<gtkmm/textview.h>
#include<gtkmm/filechooserdialog.h>
#include<string>
#include<fstream>

#include"voice.h"
#include"expression.h"

CalculatorMainWidget::CalculatorMainWidget(): voiceOn(true)
{
    glade = Gtk::Builder::create_from_file("ui.glade", "one-tab");

    add(*get_widget<Gtk::Grid>("one-tab"));

    const char* buttons[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "+", "-", ".","*", "/", "()", "^", "%", "!" };
    for (size_t i = 0; i < sizeof(buttons) / sizeof(*buttons); i++) {
        Gtk::Button* button = get_widget<Gtk::Button>(std::string("button-") + buttons[i]);

        button->signal_clicked().connect(
                sigc::bind<std::string>( sigc::mem_fun(*this, &CalculatorMainWidget::on_insert_button_clicked), buttons[i])   );

        button->signal_clicked().connect(
                sigc::bind<std::string>( sigc::mem_fun(*this, &CalculatorMainWidget::on_button_clicked_speech), buttons[i])   );
    }

    Gtk::Button* button = get_widget<Gtk::Button>("button-AC");
    button->signal_clicked().connect(sigc::mem_fun(*this, &CalculatorMainWidget::on_AC_button_clicked));
    button->signal_clicked().connect(
            sigc::bind<std::string>( sigc::mem_fun(*this, &CalculatorMainWidget::on_button_clicked_speech), "AC")    );

    button = get_widget<Gtk::Button>("button-=");
    button->signal_clicked().connect(sigc::mem_fun(*this, &CalculatorMainWidget::on_cal_button_clicked));

    get_widget<Gtk::Button>("button-del")->signal_clicked().connect(sigc::mem_fun(*this, &CalculatorMainWidget::on_del_button_clicked));

    get_widget<Gtk::Button>("button-load")->signal_clicked().connect(sigc::mem_fun(*this, &CalculatorMainWidget::on_load));
    get_widget<Gtk::Button>("button-save")->signal_clicked().connect(sigc::mem_fun(*this, &CalculatorMainWidget::on_save));


    textView = get_widget<Gtk::TextView>("textView-showExpr");
    textView->set_hexpand(true);
    textView->set_vexpand(true);
}

void CalculatorMainWidget::on_del_button_clicked()
{
    auto buffer = textView->get_buffer();
    std::string text = buffer->get_text();
    auto beforeCursor = text.substr(0, buffer->property_cursor_position());
    auto afterCursor = text.substr(buffer->property_cursor_position(), text.size());

    if (!beforeCursor.empty() && beforeCursor[beforeCursor.size() - 1] == '(') 
        if (!afterCursor.empty() && afterCursor[0] == ')')
            afterCursor =  afterCursor.substr(1);

    beforeCursor = beforeCursor.substr(0, beforeCursor.size() - 1);

    buffer->set_text(beforeCursor + afterCursor);
    buffer->place_cursor(buffer->get_iter_at_offset(beforeCursor.size()));
    textView->grab_focus();

}

void CalculatorMainWidget::on_insert_button_clicked(std::string str)
{
    auto buffer = textView->get_buffer();

    buffer->insert_at_cursor(str);

    if (str == "()")
        buffer->place_cursor(buffer->get_iter_at_offset(buffer->property_cursor_position() - 1));


    // 按钮按下时按钮是焦点，文本框就失去了焦点，也就看不到光标位置了。下面这句将其设回焦点
    textView->grab_focus();

}

void CalculatorMainWidget::on_AC_button_clicked()
{
    textView->get_buffer()->set_text("");
}

void CalculatorMainWidget::on_cal_button_clicked()
{
    ExpressionExecuter executer(textView->get_buffer()->get_text());
    auto res = executer.calculate();
    textView->get_buffer()->set_text(res);
    if (voiceOn) {
        auto equal = tts_manage(new OperatorTTS("="));
        equal->speech();
        equal->wait();

        auto tts = tts_manage(new NumberTTS(textView->get_buffer()->get_text()));
        tts->speech();
    }
}

void CalculatorMainWidget::on_button_clicked_speech(std::string op)
{
    if (voiceOn) {
        auto opTTS = tts_manage(new OperatorTTS(op));
        opTTS->speech();
    }
}

void CalculatorMainWidget::on_load()
{
    auto filepath = choose_file();
    std::fstream file(filepath);
    std::string expr;
    getline(file, expr);
    textView->get_buffer()->set_text(expr);
}

void CalculatorMainWidget::on_save()
{
    
    auto filepath = choose_file();
    std::fstream file(filepath, std::fstream::out);
    file << textView->get_buffer()->get_text();

}

void CalculatorMainWidget::set_is_voice_on(bool is)
{
    voiceOn = is;

}


std::string choose_file(void)
{
// http://www.lucidarme.me/?p=4319
    Gtk::FileChooserDialog dialog("选择文件");
    dialog.add_button("_取消", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_选择", Gtk::RESPONSE_OK);
    dialog.run();
    return dialog.get_filename();
}
