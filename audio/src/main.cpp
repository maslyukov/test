/*
 * main.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: maslyukov
 */
#include <audio/Audio.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <audio/WAVParser.h>
using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "use with PCM file\n";
        return -1;
    }
    string input(argv[1]);
    std::fstream in(input, std::ios_base::in | std::ios_base::binary);
    vector<unsigned char> pcm;
//    int fsize = in.tellg();
//    in.seekg(0, std::ios_base::end);
//    fsize = (int)in.tellg() - fsize;
//    in.seekg(0, std::ios_base::beg);

    while (/*--fsize ||*/in.good()) {
        pcm.push_back(in.get());
    }
    in.close();
    WAVParser wav(pcm.data());

    cout << "Settings : \n";
    cout << "\tbit per second " << wav.settings().BitsPerSample << endl;
    cout << "\tnumber of channels " << wav.settings().NumChannels << endl;
    cout << "\tsamples rate " << wav.settings().SampleRate << endl;
    cout << "\tsize " << wav.size() << endl;
    OpenSL::Audio a(wav.settings());
    a.set(0, wav.data(), wav.size());
    a.play();
    while (1) {
    }
    return 0;
}
