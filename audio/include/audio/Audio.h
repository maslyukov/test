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


class Player {

};



class Engine {
    SLObjectItf object;
public:
    SLEngineItf engine;
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





