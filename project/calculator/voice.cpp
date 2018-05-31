
#include<assert.h>
#include<stdio.h>
#include"voice.h"



TTS::TTS(): fpSpeech(NULL)
{
}


void TTS::wait()
{
    if (fpSpeech != NULL)
        fclose(fpSpeech);
}

void TTS::add_unit(std::string unit)
{
    units.push_back(unit);
}


void TTS::play()
{
    assert(fpSpeech == NULL);

    std::string namesPara = "";
    for (auto it = units.begin(); it != units.end(); ++it) {
        auto filename = unit_to_filename(*it);
        if (filename == "")
            fprintf(stderr, "警告：不认识的音节单元%s\n", (*it).c_str());
        namesPara += "ogg/" + filename  + ".ogg";
        namesPara += " ";
    }

    auto cmd = std::string("mplayer ") + namesPara + " >> /dev/null";
    fpSpeech = popen(cmd.c_str(), "w");
}


void TTS::speech()
{
}

std::string TTS::unit_to_filename(std::string unit)
{
    const char* names[][2] = { {"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"},
                               {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, 
                               {"10", "10"}, {"100", "100"}, {"1000", "1000"}, {"10000", "10000"},
                               {"100000000", "100000000"},
                               {"+", "plus"}, {"-", "minus"}, {".", "point"}, {"*", "multiply"}, {"/", "divide"},
                               {"()", "bracket"}, {"^", NULL}, {"%", "percent"}, {"!", "factorial"}, {"AC", "clear"},
                               {"=", "equal"} };
    for (size_t i = 0; i < sizeof(names)/sizeof(*names); i++) {
        if (unit == names[i][0] && names[i][1] != NULL) {
            return names[i][1];
        }
    }

    return "";
}

TTS::~TTS()
{
    if (fpSpeech != NULL) {
        // 加上这一句会在对象析构时停止播放
        fprintf(fpSpeech, "q");
        pclose(fpSpeech);
    }
}




OperatorTTS::OperatorTTS(std::string op): op(op)
{
}

void OperatorTTS::speech()
{
    add_unit(op);
    play();
}

OperatorTTS::~OperatorTTS()
{
}




NumberTTS::NumberTTS(std::string num): number(num)
{
}

void NumberTTS::speech()
{
    add_units();
    play();
}

NumberTTS::~NumberTTS()
{
}

void NumberTTS::add_units_for_less_10000()
{
    auto num = number;
    const char* bits[] = {NULL, NULL, "10", "100", "1000"};
    while (!num.empty()) {
        if (num[0] != '0') {
            add_unit(num.substr(0, 1));
            if (bits[num.size()])
                add_unit(bits[num.size()]);
        }

        num = num.substr(1);
    }
}

void NumberTTS::add_units()
{
    //FIXME: 一万以上的不能用， 还有中有0的不能正常读出0
    add_units_for_less_10000();
}


/****************************************/

static void atexit_TTSMemoryManager_destroy(void)
{
    delete TTSMemoryManager::get_instance();
}

TTSMemoryManager* TTSMemoryManager::instance = NULL;
TTSMemoryManager* TTSMemoryManager::get_instance()
{
    if (instance == NULL) {
        instance = new TTSMemoryManager;
        atexit(&atexit_TTSMemoryManager_destroy);
    }
    return instance;
}


TTSMemoryManager::~TTSMemoryManager()
{
    printf("来测试下析构\n");
    while (objs.size() > 0) {
        delete objs.front();
        objs.pop();
    }
}

void TTSMemoryManager::add(TTS* obj)
{
    if (objs.size() >= 100) {
        delete objs.front();
        objs.pop();
    }
    objs.push(obj);
}

TTSMemoryManager::TTSMemoryManager()
{
}
