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
    const SLInterfaceID ids[] = {SL_IID_VOLUME};
    const SLboolean req[] = {SL_BOOLEAN_FALSE};
//    cout << string(__func__) + ": creates output mix" << endl;
    if ((result = eng.object()->CreateOutputMix(eng.pointer(), &this_object, 1,
            ids, req)) != SL_RESULT_SUCCESS) {
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
            SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
//            SL_IID_BUFFERQUEUE,             // Buffer queue interface (pointer)
//            SL_IID_VOLUME                   // Volume interface (pointer)
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
    object()->Destroy(this_object);
}


//==============================================================================
void callback(SLAndroidSimpleBufferQueueItf bq, void *context) {
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
            SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &bqPlayerBufferQueue))
            != SL_RESULT_SUCCESS) { //SL_IID_BUFFERQUEUE
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
//    static int queued = 0;
    int result;
    if(pcm.size() < bufferSizeLower) {
        player->setState(Player::State::Pause);
        (*bqPlayerBufferQueue)->Clear(bqPlayerBufferQueue);
        cout << "Pause\n";
        return;
    }
    pcm_play = pcm;
    pcm.clear();
//    if(que.empty()) {
//        player->setState(Player::State::Stopped);
//        return;
//    }
//    pcm.clear();
//    while(!que.empty()) {
//        pcm.push_back(que.front());
//        que.pop();
//    }

//    queued = que.size();
    if ((result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue,
            (const void*) pcm_play.data(), pcm_play.size()/*<<1*/))//*2
            != SL_RESULT_SUCCESS) {
        stringstream ss;
        ss << result;
        throw runtime_error(
                string(__func__) + ": fail to enqueue - " + ss.str());
    }
//    if(++track >= pcms.size())
//        track = 0;
//    if ((result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue,
//            (const void*) pcms.at(track).data(), pcms.at(track).size()/*<<1*/))//*2
//            != SL_RESULT_SUCCESS) {
//        stringstream ss;
//        ss << result;
//        throw runtime_error(
//                string(__func__) + ": fail to enqueue - " + ss.str());
//    }

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
    for (int i = 0; i < size; ++i) {
        this->pcm.push_back(pcm[i]);
//        que.push(pcm[i]);
    }
}
//------------------------------------------------------------------------------
void Audio::remove(int index) {
}
//------------------------------------------------------------------------------
void Audio::play() {
    if(!isPlay() && (pcm.size() > bufferSizeHigher) ) {
        cout << "Palying\n";
        player->setState(Player::State::Palying);
        enqueue();
    }
}

//------------------------------------------------------------------------------
bool Audio::isPlay() {
    return player->getState() == Player::State::Palying;
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

