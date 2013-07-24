/*
 * Audio.h
 *
 *  Created on: Jul 24, 2013
 *      Author: maslyukov
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

namespace OpenSL {


class Object {
protected:
    SLObjectItf this_object;
public:
    Object(SLObjectItf object);
    ~Object();
    auto get()->decltype(*this_object);
    auto addr()->decltype(this_object);
};

class OutputMix : public Object {
    Engine& eng;
public:
    OutputMix(Engine& eng);
    ~OutputMix();
};

class Engine {
    SLObjectItf this_object;
    SLEngineItf this_engine;
    auto getObject()->decltype(*this_object);
public:
    auto get()->decltype(*this_engine);
    auto addr()->decltype(this_engine);
    Engine();
    ~Engine();
};






class Audio {
    SLEngineItf engineEngine;
    SLObjectItf engineObject;
    void init();
    void shutdown();
    friend static void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq,
            void *context);
public:
    Audio();
    virtual ~Audio();
};

}

#endif /* AUDIO_H_ */





