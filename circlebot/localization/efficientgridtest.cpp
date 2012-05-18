#include <iostream>
#include <string.h>
using namespace std;

const size_t width = 5;
const size_t height = 5;
int world[width][height] = {
    {0, 10, 20, 30, 40},
    {1, 11, 21, 31, 41},
    {2, 12, 22, 32, 42},
    {3, 13, 23, 33, 43},
    {4, 14, 24, 34, 44},
};

const size_t buflen = width+height-1;
int buf[buflen];
int main() {
    memset(buf, 0xff, buflen*sizeof(buf[0]));

    // Print out the world
    for (size_t i=0; i<width; i++) {
        cout << "{" << world[i][0];
        for (size_t j=1; j<height; j++) {
            cout << ", " << world[i][j];
        }
        cout << "}" <<  endl;
    }

    // Test out the buffer generator
    for (size_t i=0; i<width; i++) {
        for (size_t j=0; j<i; j++) {
            buf[j] = world[i][j];
            buf[((i*2)-j)] = world[j][i];
        }
        buf[i] = world[i][i];

        cout << "{" << buf[0];
        for (size_t j=1; j<buflen; j++) {
            cout << ", " << buf[j];
        }
        cout << "}" <<  endl;
    }

    // make each number the sum of its neighbors
    // Print out what it should be

    cout << "{22, 63, ..." << endl;

    for (size_t i=0; i<width; i++) {
        for (size_t j=0; j<i; j++) {
            buf[j] = world[i][j];
            buf[((i*2)-j)] = world[j][i];
        }
        buf[i] = world[i][i];

        cout << "{" << buf[0];
        for (size_t j=1; j<buflen; j++) {
            cout << ", " << buf[j];
        }
        cout << "}" <<  endl;
    }
}
