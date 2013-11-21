/*
 * Audio.h
 *
 *  Created on: Jul 24, 2013
 *      Author: maslyukov
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <map>
#include <vector>
#include <memory>
#include <queue>
#include <audio/WAVParser.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <thread>
#include <atomic>



class spinlock_mutex {
    std::atomic_flag flag;
public:
    spinlock_mutex() :
            flag(ATOMIC_FLAG_INIT) {
    }
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire));
    }
    void unlock() {
        flag.clear(std::memory_order_release);
    }
};


namespace OpenSL {

using namespace std;

//==============================================================================
class Engine {
    SLObjectItf this_object;
    SLEngineItf this_engine;
    auto getObject()->decltype(*this_object);
public:
    auto object()->decltype(*this_engine);
    auto pointer()->decltype(this_engine);
    Engine();
    ~Engine();
};

//==============================================================================
class Object {
protected:
    SLObjectItf this_object;
public:
    Object(SLObjectItf object);
    Object() :
            this_object(nullptr) {
    }
    ~Object();
    auto object()->decltype(*this_object);
    auto pointer()->decltype(this_object);
};

//==============================================================================
class OutputMix: public Object {
    Engine& eng;
public:
    OutputMix(Engine& eng);
    ~OutputMix();
};

//==============================================================================
class Player: public Object {
    Engine& eng;

    OutputMix outputMix;

    SLPlayItf bqPlayerPlay;
    SLVolumeItf bqPlayerVolume;
public:
    enum class State {
        Stop = (SLuint32)1, Pause = (SLuint32)2, Play = (SLuint32)3
    };
    State getState();
    void setState(State state);
    void setVolume(int millibel);
    Player(Engine& eng, const Settings& settings);
    ~Player();

};

//==============================================================================
class Audio {
    int track;
    unique_ptr<Engine> eng;
    unique_ptr<Player> player;
    SLBufferQueueItf bqPlayerBufferQueue;
    vector<vector<unsigned char> > pcms;
    vector<unsigned char> pcm_play;
    vector<unsigned char> pcm;
    queue<unsigned char> que;
    friend void callback(SLBufferQueueItf bq, void *context);
    void enqueue();
//    bool clearReq;
    spinlock_mutex spinlock;
    enum {BUFFER_SIZE_HIGH = 0x1000};
    enum {BUFFER_SIZE_LOW  = 0x0800};
    Audio(Audio& a) = delete;
    Audio() = delete;
public:
    void set(int index, const vector<unsigned char>& pcm);
    void set(int index, const unsigned char* pcm, int size);
    void add(const unsigned char* pcm, int size);
    void remove(int index);
    void play();
    bool isPlay();
    void stop();
    void volume(int millibel);
    Audio(const Settings& settings);
    ~Audio();
};

} //namespace OpenSL

#endif /* AUDIO_H_ */

