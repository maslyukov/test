/*
 * Audio.h
 *
 *  Created on: Jul 24, 2013
 *      Author: maslyukov
 */

#ifndef AUDIO_H_
#define AUDIO_H_

namespace OpenSL {

#include <vector>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

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
    Object() : this_object(nullptr){}
    ~Object();
    auto object()->decltype(*this_object);
    auto pointer()->decltype(this_object);
};

//==============================================================================
class OutputMix : public Object {
    Engine& eng;
public:
    OutputMix(Engine& eng);
    ~OutputMix();
};

//==============================================================================
class Player : public Object {
    Engine& eng;

    OutputMix outputMix;

    SLPlayItf bqPlayerPlay;
    SLVolumeItf bqPlayerVolume;
public:
    using state_t = enum State {
        Stopped = 1,
        Pause = 2,
        Palying = 3
    };
    void setState(state_t state);
    void setVolume(int millibel);
    Player(Engine& eng);
    ~Player();

};

//==============================================================================
class Audio {
    Engine eng;
    Player player;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
    vector<unsigned short> pcm;
    friend void callback(SLAndroidSimpleBufferQueueItf bq, void *context);
public:
    void play(const vector<unsigned short>& pcm);
    void stop();
    void volume(int millibel);
    Audio();
    ~Audio();
};

}//namespace OpenSL

#endif /* AUDIO_H_ */





