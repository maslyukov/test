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

using namespace OpenSL;
using namespace std;

Object::Object(SLObjectItf object) :
        this_object(object) {

}
Object::~Object() {

}
auto Object::get()->decltype(*this_object) {
    return *this_object;
}
auto Object::addr()->decltype(this_object) {
    return this_object;
}

//==============================================================================
Engine::Engine() {
    if (slCreateEngine(&this_object, 0, nullptr, 0, nullptr,
            nullptr) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to create");
    }
    if (getObject()->Realize(this_object, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to realize");
    }
    if (getObject()->GetInterface(this_object, SL_IID_ENGINE,
            &this_engine) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to get inteface");

    }
}
Engine::~Engine() {
    getObject()->Destroy(this_object);
}
auto Engine::getObject()->decltype(*this_object) {
    return *this_object;
}
auto Engine::get()->decltype(*this_engine) {
    return *this_engine;
}
auto Engine::addr()->decltype(this_engine) {
    return this_engine;
}

//==============================================================================
OutputMix::OutputMix(Engine& eng) :
        eng(eng) {
    const SLInterfaceID ids[1] = { SL_IID_ENVIRONMENTALREVERB };
    const SLboolean req[1] = { SL_BOOLEAN_FALSE };
    if (eng.get()->CreateOutputMix(eng.addr(), &this_object, 1, ids,
            req) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to create");
    }
    if (this->get()->Realize(this_object, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS) {
        throw runtime_error(string(__func__) + ": fail to realize");
    }
}

OutputMix::~OutputMix() {

}

