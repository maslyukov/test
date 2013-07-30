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
    const vector<short> pcm1((short*)&android[0], (short*)&android[sizeof(android) - 1]);
    const vector<short> pcm2((short*)hello, (short*)&hello[sizeof(hello) - 1]);
    cout << "m1" << endl;
    OpenSL::Audio a;
    a.add(0,pcm1);
    a.add(1,pcm2);
    a.play();
    cout << "m2" << endl;
    while (1) {
    }
    return 0;
}
