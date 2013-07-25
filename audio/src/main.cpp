/*
 * main.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: maslyukov
 */
#include <audio/Audio.h>
#include <vector>

static const char hello[] = {
#include <samples/hello_clip.h>
};

static const char android[] = {
#include <samples/android_clip.h>
};

using namespace std;
int main() {
    const vector<unsigned short> pcm(hello, &hello[sizeof(hello)-1]);
    OpenSL::Audio().play(pcm);
    while(1) {
    }
    return 0;
}
