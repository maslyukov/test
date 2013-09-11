/*
 * WAVParser.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: maslyukov
 */

#include <audio/WAVParser.h>
#include <iostream>

using namespace std;

WAVParser::WAVParser(unsigned char* wav) : h((header*)wav) {
    sett.BitsPerSample = h->BitsPerSample;
    sett.NumChannels = h->NumChannels;
    sett.SampleRate = h->SampleRate;
}

WAVParser::~WAVParser() {

}


Settings WAVParser::settings() {
    return sett;
}

unsigned char* WAVParser::data() {
    return h->Data;
}

int WAVParser::size() {
    return h->Subchunk2Size;
}
