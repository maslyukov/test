/*
 * Audio.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: maslyukov
 */

#include <audio/Audio.h>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

namespace OpenSL {

//==============================================================================
Object::Object(SLObjectItf object) :
        this_object(object) {
}

//------------------------------------------------------------------------------
Object::~Object() {

}

//------------------------------------------------------------------------------
auto Object::object()->decltype(*this_object) {
    return *this_object;
}

//------------------------------------------------------------------------------
auto Object::pointer()->decltype(this_object) {
    return this_object;
}
//==============================================================================

//==============================================================================
Engine::Engine() {
    if (slCreateEngine(&this_object, 0, nullptr, 0, nullptr,
            nullptr) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to create");
    }
    if (getObject()->Realize(this_object, SL_BOOLEAN_FALSE)
            != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to realize");
    }
    if (getObject()->GetInterface(this_object, SL_IID_ENGINE,
            &this_engine) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to get inteface");
    }
}

//------------------------------------------------------------------------------
Engine::~Engine() {
    getObject()->Destroy(this_object);
}

//------------------------------------------------------------------------------
auto Engine::getObject()->decltype(*this_object) {
    return *this_object;
}

//------------------------------------------------------------------------------
auto Engine::object()->decltype(*this_engine) {
    return *this_engine;
}

//------------------------------------------------------------------------------
auto Engine::pointer()->decltype(this_engine) {
    return this_engine;
}
//==============================================================================

//==============================================================================
OutputMix::OutputMix(Engine& eng) :
        eng(eng) {
    const SLInterfaceID ids[1] = { SL_IID_ENVIRONMENTALREVERB };
    const SLboolean req[1] = { SL_BOOLEAN_FALSE };
    if (eng.object()->CreateOutputMix(eng.pointer(), &this_object, 1, ids,
            req) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to create");
    }
    if (object()->Realize(this_object, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to realize");
    }
}

//------------------------------------------------------------------------------
OutputMix::~OutputMix() {
    object()->Destroy(this_object);
}
//==============================================================================

//==============================================================================
Player::Player(Engine& eng) :
    eng(eng), outputMix(eng) {
    const int interfaces = 2;
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,    // locator type
            2                                           // number of buffers
    };
    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,              // Format type (PCM)
            1,                              // Number of channels
            SL_SAMPLINGRATE_8,              // Samples per second (8kHz)
            SL_PCMSAMPLEFORMAT_FIXED_16,    // Bits per second
            SL_PCMSAMPLEFORMAT_FIXED_16,    // Container size
            SL_SPEAKER_FRONT_CENTER,        // Channel mask
            SL_BYTEORDER_LITTLEENDIAN       // Endianness (little endian)
    };
    SLDataSource audioSrc = {
            &loc_bufq,                      // pointer to locator
            &format_pcm                     // pointer to format
    };
    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {
            SL_DATALOCATOR_OUTPUTMIX,       // Locator type
            outputMix.pointer()             // output mix
//            outputMixObject               // output mix
    };
    SLDataSink audioSnk = {
            &loc_outmix,                    // pointer to locator
            NULL                            // pointer to format
    };
    // create audio player
    const SLInterfaceID ids[interfaces] = {  // Array of 3 Interfaces
            SL_IID_BUFFERQUEUE,             // Buffer queue interface (pointer)
            SL_IID_VOLUME                   // Volume interface (pointer)
    };
    const SLboolean req[interfaces] = {              // Array of 3 flags
            SL_BOOLEAN_TRUE,                // (SLboolean) 0x00000001
            SL_BOOLEAN_TRUE                 // (SLboolean) 0x00000001
    };
    if (eng.object()->CreateAudioPlayer(eng.pointer(), &this_object, &audioSrc,
            &audioSnk, interfaces, ids, req) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to create");
    }
    if (object()->Realize(this_object,SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to realize");
    }
    if (object()->GetInterface(this_object, SL_IID_PLAY, &bqPlayerPlay)
            != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) +": fail to get player interface");
    }
    if (object()->GetInterface(this_object, SL_IID_VOLUME, &bqPlayerVolume)
            != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) +": fail to get volume interface");
    }
    setState(State::Stopped);

}

//------------------------------------------------------------------------------
Player::~Player() {
    object()->Destroy(this_object);
}

//------------------------------------------------------------------------------
void Player::setState(state_t state) {
    (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, state);
}

//------------------------------------------------------------------------------
void Player::setVolume(int millibel) {
    if ((*bqPlayerVolume)->SetVolumeLevel(bqPlayerVolume, millibel)
            != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to set volume level");
    }
}

//==============================================================================

//==============================================================================
void callback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    auto a = (Audio*)context;
    if ((*a->bqPlayerBufferQueue)->Enqueue(a->bqPlayerBufferQueue,
            a->pcm.data(), a->pcm.size()) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to enqueue");
    }
}

//------------------------------------------------------------------------------
Audio::Audio() : eng(), player(eng) {
    if (player.object()->GetInterface(player.pointer(), SL_IID_BUFFERQUEUE,
            &bqPlayerBufferQueue) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to get interface");
    }

    if ((*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, callback,
            (void*)this) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to register callback");
    }
}

//------------------------------------------------------------------------------
Audio::~Audio() {
}

//------------------------------------------------------------------------------
void Audio::play(const vector<unsigned short>& pcm) {
    this->pcm = pcm;
    player.setState(Player::State::Palying);
}

//------------------------------------------------------------------------------
void Audio::stop() {
    player.setState(Player::State::Stopped);
}

//------------------------------------------------------------------------------
void Audio::volume(int millibel) {
    player.setVolume(millibel);
}
//==============================================================================

} //namespace OpenSL


















