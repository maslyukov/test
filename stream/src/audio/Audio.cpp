/*
 * Audio.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: maslyukov
 */
#include <audio/Audio.h>
#include <stdexcept>
#include <cstdio>
#include <array>
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
    const SLInterfaceID pIDs[1] = {SL_IID_ENGINE};
    const SLboolean pIDsRequired[1]  = {SL_BOOLEAN_TRUE};
    cout << string(__func__) + ": SL - creates engine" << endl;
    if (slCreateEngine(&this_object, 0, nullptr, 1, pIDs,
            pIDsRequired) != SL_RESULT_SUCCESS) {
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
    getObject()->Destroy(this_object);
}


//==============================================================================
OutputMix::OutputMix(Engine& eng) :
        eng(eng) {
    int result;
    const int interfaces = 0;
    const SLInterfaceID ids[interfaces] = {};//{SL_IID_VOLUME};
    const SLboolean req[interfaces] = {};//{SL_BOOLEAN_FALSE};
    if ((result = eng.object()->CreateOutputMix(eng.pointer(), &this_object,
            interfaces, ids, req)) != SL_RESULT_SUCCESS) {
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
Player::Player(Engine& eng, const Settings& settings) :
        eng(eng), outputMix(eng) {
    const int interfaces = 1;
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
    SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,    // locator type
            1                                   // number of buffers
            };
    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,              // Format type (PCM)
            (SLuint32)settings.NumChannels,                              // Number of channels
            (SLuint32)settings.SampleRate*1000,  // Samples per second (8kHz)
            (SLuint32)settings.BitsPerSample,    // Bits per second
            (SLuint32)settings.BitsPerSample,    // Container size
            (settings.NumChannels == 1) ?
                (SL_SPEAKER_FRONT_CENTER) :
                (SL_SPEAKER_FRONT_RIGHT | SL_SPEAKER_FRONT_LEFT),
            SL_BYTEORDER_LITTLEENDIAN,       // Endianness (little endian)
            };
    SLDataSource audioSrc = {
            &loc_bufq,                      // pointer to locator
            &format_pcm                     // pointer to format
            };
    // configure audio sink
    SLDataLocator_OutputMix loc_outmix;
    loc_outmix.outputMix = outputMix.pointer();
    loc_outmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;

    SLDataSink audioSnk = {
            &loc_outmix,                    // pointer to locator
            NULL                            // pointer to format
            };
    // create audio player
    const SLInterfaceID ids[interfaces] = { // Array of 3 Interfaces
            SL_IID_BUFFERQUEUE,             // Buffer queue interface (pointer)
//            SL_IID_VOLUME                   // Volume interface (pointer)
//            SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
            };
    const SLboolean req[interfaces] = {     // Array of 3 flags
            SL_BOOLEAN_TRUE,                // (SLboolean) 0x00000001
//            SL_BOOLEAN_TRUE,                // (SLboolean) 0x00000001
//            SL_BOOLEAN_TRUE                 // (SLboolean) 0x00000001
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
//    cout << string(__func__) + ": gets volume interface" << endl;
//    if (object()->GetInterface(this_object, SL_IID_VOLUME,
//            &bqPlayerVolume) != SL_RESULT_SUCCESS) {
//        throw runtime_error(
//                string(__func__) + ": fail to get volume interface");
//    }
}

//------------------------------------------------------------------------------
void Player::setState(State state) {
    (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, static_cast<SLuint32>(state));
}

//------------------------------------------------------------------------------
void Player::setVolume(int millibel) {
    if ((*bqPlayerVolume)->SetVolumeLevel(bqPlayerVolume,
            millibel) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to set volume level");
    }
}

//------------------------------------------------------------------------------
Player::State Player::getState() {
    State state;
    (*bqPlayerPlay)->GetPlayState(bqPlayerPlay,
            reinterpret_cast<SLuint32*>(&state));
    return state;
}

//------------------------------------------------------------------------------
Player::~Player() {
    cout << string(__func__) + ": Destroying player" << endl;
    object()->Destroy(this_object);
}


//==============================================================================
void callback(SLBufferQueueItf bq, void *context) {
    auto a = (Audio*) context;
//    cout << string(__func__) + ": enter" << endl;
    a->enqueue();
}

//------------------------------------------------------------------------------
Audio::Audio(const Settings& settings) :
        track(0) {
    int result;
    eng.reset(new Engine());
    player.reset(new Player(*eng.get(), settings));
    cout << string(__func__) + ": gets queue interface" << endl;
    if ((result = player->object()->GetInterface(player->pointer(),
            SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue)) //
            != SL_RESULT_SUCCESS) { //SL_IID_ANDROIDSIMPLEBUFFERQUEUE
        stringstream ss;
        ss << result;
        throw runtime_error(
                string(__func__) + ": fail to get interface - " + ss.str());
    }
    player->setState(Player::State::Stop);

    if ((result = (*bqPlayerBufferQueue)->Clear(bqPlayerBufferQueue))
             != SL_RESULT_SUCCESS) {
        stringstream ss;
        ss << result;
        throw runtime_error(
                string(__func__) + ": fail to clear");
    }

    cout << string(__func__) + ": registers callback" << endl;
    if ((result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue,
            callback, (void*) this)) != SL_RESULT_SUCCESS) {
        stringstream ss;
        ss << result;
        throw runtime_error(
                string(__func__) + ": fail to register callback - " + ss.str());
    }
}

//------------------------------------------------------------------------------
void Audio::enqueue() {
//    static int emty_buf_events = 0;
    int result = SL_RESULT_SUCCESS;

    pcm_play.clear();
    spinlock.lock();
    pcm_play = pcm;
    pcm.clear();
    spinlock.unlock();

    do {
        if (!pcm_play.size()) {
            player->setState(Player::State::Stop);
            break;
        }
        if ((result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue,
                static_cast<const void*>(pcm_play.data()), pcm_play.size()))
                != SL_RESULT_SUCCESS) {
            player->setState(Player::State::Stop);
            break;
        }
    } while (false);

}

//------------------------------------------------------------------------------
void Audio::set(int index, const vector<unsigned char>& pcm) {
    if (index >= pcms.size())
        pcms.resize(index+1);
    pcms.at(index) = pcm;
}
//------------------------------------------------------------------------------
void Audio::set(int index, const unsigned char* pcm, int size) {
    if (index >= pcms.size())
        pcms.resize(index+1);
    for (int i = 0; i < size; ++i)
        pcms.at(index).push_back(pcm[i]);
}
//------------------------------------------------------------------------------
void Audio::add(const unsigned char* pcm, int size) {
    spinlock.lock();
    this->pcm.reserve(size);
    for (int i = 0; i < size; ++i) {
        this->pcm.push_back(pcm[i]);
    }
    spinlock.unlock();
}
//------------------------------------------------------------------------------
void Audio::remove(int index) {
}
//------------------------------------------------------------------------------
void Audio::play() {
    int size;
    spinlock.lock();
    size = pcm.size();
    spinlock.unlock();
    if(!isPlay() ) {
        enqueue();
        player->setState(Player::State::Play);
//        cout << "Playing\n";
    }
}

//------------------------------------------------------------------------------
bool Audio::isPlay() {
    return player->getState() == Player::State::Play;
}

//------------------------------------------------------------------------------
void Audio::stop() {
    player->setState(Player::State::Stop);
}

//------------------------------------------------------------------------------
void Audio::volume(int millibel) {
    player->setVolume(millibel);
}

//------------------------------------------------------------------------------
Audio::~Audio() {
    int result;
    cout << string(__func__) + ": Clearing buffer" << endl;
    if ((result = (*bqPlayerBufferQueue)->Clear(bqPlayerBufferQueue))
             != SL_RESULT_SUCCESS) {
        stringstream ss;
        ss << result;
        throw runtime_error(
                string(__func__) + ": fail to clear");
    }
}

}//namespace OpenSL

