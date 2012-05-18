#include <Arduino.h>
// Maximum size of an argument list
const size_t maxArgListLen = 4;

// Maximum length for a packet (does not include start and end markers)
// could be r,-1000,-1000,-1000
const size_t maxPacketLen = 19;

// Character that marks that start of a packet
const char packetStart = '<';

// Character that marks the end of a packet
const char packetEnd = '>';

// Describes the syntax for the r command
namespace rbtStx {
// Name given to address command
    const char cmd = 'r';
// Length of command
    const size_t length = 3;
// Position of the x value
    const size_t xPos = 0;
// Position of the y value
    const size_t yPos = 1;
// Position of the z value
    const size_t zPos = 2;
}

// Describes the syntax for the m command
namespace mtrStx {
// Name given to address command
    const char cmd = 'm';
// Length of command
    const size_t length = 3;
// Position of the first motor value
    const size_t mtr0Pos = 0;
// Position of the second motor value
    const size_t mtr1Pos = 1;
// Position of the third motor value
    const size_t mtr2Pos = 2;
}

// Describes the syntax for the m command
namespace rawStx {
// Name given to address command
    const char cmd = 'd';
// Length of command
    const size_t length = 3;
// Position of the first motor value
    const size_t mtr0Pos = 0;
// Position of the second motor value
    const size_t mtr1Pos = 1;
// Position of the third motor value
    const size_t mtr2Pos = 2;
}
