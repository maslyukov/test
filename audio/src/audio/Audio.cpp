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
#include <iostream>
#include <sstream>

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
Engine::Engine() {
    cout << string(__func__) + ": SL - creates engine" << endl;
    if (slCreateEngine(&this_object, 0, nullptr, 0, nullptr,
            nullptr) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to create");
    }
    cout << string(__func__) + ": SL - realizes engine" << endl;
    if (getObject()->Realize(this_object, SL_BOOLEAN_FALSE)
            != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to realize");
    }
    cout << string(__func__) + ": SL - gets interface engine" << endl;
    if (getObject()->GetInterface(this_object, SL_IID_ENGINE,
            &this_engine) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to get inteface");
    }
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

//------------------------------------------------------------------------------
Engine::~Engine() {
    cout << string(__func__) + ": SL - destroy engine" << endl;
    getObject()->Destroy(this_object);
}


//==============================================================================
OutputMix::OutputMix(Engine& eng) :
        eng(eng) {
    int result;
//    cout << string(__func__) + ": creates output mix" << endl;
    if ((result = eng.object()->CreateOutputMix(eng.pointer(), &this_object,
            0, nullptr, nullptr)) != SL_RESULT_SUCCESS) {
        stringstream ss;
        ss << result;
        throw runtime_error(string(__func__) + ": fail to create - "+ ss.str());
    }
    cout << string(__func__) + ": realizes output mix" << endl;
    if ((result = object()->Realize(this_object, SL_BOOLEAN_FALSE))
            != SL_RESULT_SUCCESS) {
        stringstream ss;
        ss << result;
        throw runtime_error(string(__func__) + ": fail to realize - "+ss.str());
    }
}

//------------------------------------------------------------------------------
OutputMix::~OutputMix() {
    object()->Destroy(this_object);
}


//==============================================================================
Player::Player(Engine& eng) :
        eng(eng), outputMix(eng) {
    const int interfaces = 2;
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
    SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,    // locator type
            1                                           // number of buffers
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
    SLDataLocator_OutputMix loc_outmix;
    loc_outmix.outputMix = outputMix.pointer();
    loc_outmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;

//    // = {
//    SL_DATALOCATOR_OUTPUTMIX,             // Locator type
//            outputMix.pointer()           // output mix
//            outputMixObject               // output mix
//            };
    SLDataSink audioSnk = {
            &loc_outmix,                    // pointer to locator
            NULL                            // pointer to format
            };
    // create audio player
    const SLInterfaceID ids[interfaces] = { // Array of 3 Interfaces
            SL_IID_BUFFERQUEUE,             // Buffer queue interface (pointer)
            SL_IID_VOLUME                   // Volume interface (pointer)
            };
    const SLboolean req[interfaces] = {     // Array of 3 flags
            SL_BOOLEAN_TRUE,                // (SLboolean) 0x00000001
            SL_BOOLEAN_TRUE                 // (SLboolean) 0x00000001
            };
    cout << string(__func__) + ": creates player" << endl;
    if (eng.object()->CreateAudioPlayer(eng.pointer(), &this_object, &audioSrc,
            &audioSnk, interfaces, ids, req) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to create");
    }
    cout << string(__func__) + ": realizes player" << endl;
    if (object()->Realize(this_object, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to realize");
    }
    cout << string(__func__) + ": gets player interface" << endl;
    if (object()->GetInterface(this_object, SL_IID_PLAY,
            &bqPlayerPlay) != SL_RESULT_SUCCESS) {
        throw runtime_error(
                string(__func__) + ": fail to get player interface");
    }
    cout << string(__func__) + ": gets volume interface" << endl;
    if (object()->GetInterface(this_object, SL_IID_VOLUME,
            &bqPlayerVolume) != SL_RESULT_SUCCESS) {
        throw runtime_error(
                string(__func__) + ": fail to get volume interface");
    }
}

//------------------------------------------------------------------------------
void Player::setState(state_t state) {
    (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, state);
}

//------------------------------------------------------------------------------
void Player::setVolume(int millibel) {
    if ((*bqPlayerVolume)->SetVolumeLevel(bqPlayerVolume,
            millibel) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to set volume level");
    }
}

//------------------------------------------------------------------------------
Player::~Player() {
    object()->Destroy(this_object);
}


//==============================================================================
void callback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    auto a = (Audio*) context;
    cout << string(__func__) + ": enter" << endl;
    a->enqueue();
}

//------------------------------------------------------------------------------
Audio::Audio() {
    int result;
    eng.reset(new Engine());
    player.reset(new Player(*eng.get()));
    cout << string(__func__) + ": gets queue interface" << endl;
    if ((result = player->object()->GetInterface(player->pointer(),
            SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue)) != SL_RESULT_SUCCESS) {
        stringstream ss;
        ss << result;
        throw runtime_error(
                string(__func__) + ": fail to get interface - " + ss.str());
    }
    cout << string(__func__) + ": registers callback" << endl;
    if ((result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue,
            callback, (void*) this)) != SL_RESULT_SUCCESS) {
        stringstream ss;
        ss << result;
        throw runtime_error(
                string(__func__) + ": fail to register callback - " + ss.str());
    }
//    enqueue();
//    player->setState(Player::State::Palying);
}

//------------------------------------------------------------------------------
void Audio::enqueue() {
    int result;
    if ((result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue,
            (const void*) pcm.data(), pcm.size())) != SL_RESULT_SUCCESS) {
        stringstream ss;
        ss << result;
        throw runtime_error(
                string(__func__) + ": fail to enqueue - " + ss.str());
    }
}

//------------------------------------------------------------------------------
void Audio::play(const vector<unsigned short>& pcm) {
    if (&this->pcm != &pcm)
        this->pcm = pcm;
    player->setState(Player::State::Palying);
    enqueue();
}

//------------------------------------------------------------------------------
void Audio::stop() {
    player->setState(Player::State::Stopped);
}

//------------------------------------------------------------------------------
void Audio::volume(int millibel) {
    player->setVolume(millibel);
}

//------------------------------------------------------------------------------
Audio::~Audio() {
}

}//namespace OpenSL

