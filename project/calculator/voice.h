#ifndef __VOICE_H__
#define __VOICE_H__


#include<vector>
#include<string>
#include<queue>

class TTS
{
public:
    TTS();
    virtual ~TTS();
    void wait();
    void speech();

protected:
    void add_unit(std::string unit);
    void play();


private:
    std::string unit_to_filename(std::string unit);
    std::vector<std::string> units; 

    FILE* fpSpeech;

};

class OperatorTTS: public TTS
{
public:
    OperatorTTS(std::string op);
    ~OperatorTTS();
    void speech();

private:
    std::string op;
};

class NumberTTS: public TTS
{
public:
    NumberTTS(std::string num);
    ~NumberTTS();
    void add_units();
    void add_units_for_less_10000();
    void speech();
    

private:
    std::string number;
};


/************************************/

class TTSMemoryManager
{
public:
    static TTSMemoryManager* get_instance();
private:
    static TTSMemoryManager* instance;

public:
    ~TTSMemoryManager();
    void add(TTS* obj);
private:
    TTSMemoryManager();
    std::queue<TTS*> objs;
};


template<typename T>
T* tts_manage(T* obj)
{
    TTSMemoryManager::get_instance()->add(obj);
    return obj;
}


#endif
