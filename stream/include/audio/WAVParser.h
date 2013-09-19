/*
 * WAVParser.h
 *
 *  Created on: Aug 26, 2013
 *      Author: maslyukov
 */

#ifndef WAVPARSER_H_
#define WAVPARSER_H_
#include <vector>

#pragma pack(push,1)
struct Settings {
    short NumChannels;
    short BitsPerSample;
    int SampleRate;
};
#pragma pack(pop)

class WAVParser {

    struct header {
        char ChunkID[4];
        int ChunkSize;
        char Format[4];
        char Subchunk1ID[4];
        int Subchunk1Size;
        short AudioFormat;
        short NumChannels;
        int SampleRate;
        int ByteRate;
        short BlockAlign;
        short BitsPerSample;
        char Subchunk2ID[4];
        int Subchunk2Size;
        unsigned char Data[0];
    };
    header* h;
    Settings sett;
    WAVParser(){};
public:

    WAVParser(unsigned char* wav);
    virtual ~WAVParser();
    Settings settings();
    unsigned char* data();
    int size();
};

#endif /* WAVPARSER_H_ */
