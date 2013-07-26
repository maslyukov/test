/*
 * main.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: maslyukov
 */
#include <audio/Audio.h>
#include <vector>
#include <iostream>

static const char hello[] = {
#include <samples/hello_clip.h>
        };

static const char android[] = {
#include <samples/android_clip.h>
        };

using namespace std;

int main() {
    const vector<unsigned short> pcm(hello, &hello[sizeof(hello) - 1]);
    cout << "m1" << endl;
    OpenSL::Audio a;
    a.play(pcm);
    cout << "m2" << endl;
    while (1) {
    }
    return 0;
}
