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
#include <audio/WAVParser.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

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
    typedef enum State {
        Stop = (SLuint32)1, Pause = (SLuint32)2, Playing = (SLuint32)3
    } state_t;
    void setState(state_t state);
    void setVolume(int millibel);
    Player(Engine& eng, const Settings& settings);
    ~Player();

};

//==============================================================================
class Audio {
    int track;
    unique_ptr<Engine> eng;
    unique_ptr<Player> player;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
    vector<vector<unsigned char> > pcms;
    friend void callback(SLAndroidSimpleBufferQueueItf bq, void *context);
    void enqueue();
public:
    void set(int index, const vector<unsigned char>& pcm);
    void set(int index, const unsigned char* pcm, int size);
    void remove(int index);
    void play();
    void stop();
    void volume(int millibel);
    Audio(const Settings& settings);
    ~Audio();
};

} //namespace OpenSL

#endif /* AUDIO_H_ */

