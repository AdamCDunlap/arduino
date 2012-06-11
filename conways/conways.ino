#include <LEDArray.h>

LEDArray disp(2, 3, 4);

unsigned long iterationTime = 250; // Time for each iteration
byte density = 25;                 // Density of the random grid, out of 100
bool useRandom = true;             // Display a random grid when restarting
byte deadLoops = 8;                // How many loops before restarting once dead
bool wrapEdges = false;            // Whether to wrap like a torus

byte defaultGrid[8];

// Prototypes
void serialWriteStartOfRow(byte rowNum);
byte numNeighbors(byte* grid, byte row, byte column, bool wrap);
void setup();
void loop();
bool checkSerial();
bool changeDensity(char incoming);
void ignoreNewline ();
void serialWriteDead(byte row, byte column);
void serialWriteAlive(byte row, byte column);
void serialWriteStartOfBox();
void serialWriteEndOfBox();
void serialWriteEndOfRow();
void generateRandomPattern(bool verbose);


void setup() {
    Serial.begin(9600);
    Serial.print(F(
    "Welcome to Conway's Game of life!\n"
    "We will start with a default pattern, but you can override that\n"
    "by typing into the box above.\n"
    "Make sure you set the line ending to \"Newline\"\n\n"
    "The syntax is: 8 lines of 8 characters each\n"
    "Representing each of the 8 rows and columns on the LED Matrix.\n"
    "Each 0 represents a dead cell and anything else represents a "
        "living cell.\n\n"
    "Sending 'c' cancels the current input.\n"
    "Sending 'h' displays this help.\n"
    "Sending 'd' displays the current input pattern on the matrix.\n"
    "Sending 'w' makes the display wrap around like a torus.\n"
    "Sending 'W' cancels 'w'.\n"
    "Sending 's' sends a random pattern to the display.\n"
    "Sending 'r' sends a random pattern whenever the simulation dies (defualt).\n"
    "Sending 'R' cancels 'r'.\n"
    "Sending 'D=xx' (xx is a number) changes the density of the random "
        "pattern\n"
    ));
    randomSeed(analogRead(3));
}

byte serialBuffer[8]; // Grid that user is inputting
byte randomBuffer[8]; // Stores the output of the random grid generator

void loop() {
    static byte lastGrid[8];
    static byte curGrid[8] = {128,96,192,0, 0,0,0,0}; //Start off with glider
    static byte nextGrid[8]; //Place where the next grid is calculated

    static unsigned long currentTime;
    static unsigned long lastDrawTime;
    
    static bool dead = false;
    static byte deadcounter = 0;
    static bool looping = false;
    
    currentTime = millis();
    
    if (checkSerial()) {
        dead = false;
        looping = false;
        for (byte i=0;i<8;i++) {
            curGrid[i] = defaultGrid[i] = serialBuffer[i];
        }
    }
      
    if (currentTime - lastDrawTime >= iterationTime) {
        lastDrawTime = currentTime;

        if (dead) {
            if (deadcounter == deadLoops) {
                for (byte i=0;i<8;i++) {
                    curGrid[i] = 0xff;
                }
                // Stop the simulation from running
                looping = false;
            }
            else if (deadcounter > deadLoops + 1) {
                dead = false;
                looping = false;
                deadcounter = 0;
                if (useRandom) generateRandomPattern(false);
                for (byte i=0;i<8;i++) {
                    if (useRandom) curGrid[i] = randomBuffer[i];
                    else           curGrid[i] = defaultGrid[i];
                }
            }
            deadcounter++;
        }

        // If we're either normal or in a loop, run the simulation
        if (!dead || looping) {
            for (byte j=0;j<8;j++) {
                for (byte i=0;i<8;i++) {
                    byte neighbors = numNeighbors(curGrid, j, i, wrapEdges);
                    if (bitRead(curGrid[j], i)) {
                        if (neighbors == 2 || neighbors == 3) {
                            bitSet(nextGrid[j], i);
                        }
                        else {
                            bitClear(nextGrid[j], i);
                        }
                    }
                    else {
                        if (neighbors == 3) {
                            bitSet(nextGrid[j], i);
                        }
                        else {
                            bitClear(nextGrid[j], i);
                        }
                    }
                }
            }
        }

        
        // This section of code checks to see if the simulation is either
        //  dead or in a loop

        // Start off with assuming nothing's changed but 
        //  change it if something has changed
        dead = true;
        looping = true;
        for (byte i=0;i<8;i++) {
            if (curGrid[i]  != nextGrid[i]) dead = false;
            if (lastGrid[i] != nextGrid[i]) dead = looping = false;
            lastGrid[i] = curGrid[i];
            curGrid[i] = nextGrid[i];
        }


        disp.drawPic(curGrid);
    }
}

byte numNeighbors(byte* grid, byte row, byte column, bool wrap = false) {
    byte neighbors = 0;
    for (int8_t j = -1; j <= 1; j++) {
        for (int8_t i = -1; i <= 1; i++) {
            if (i == 0 && j == 0) continue;
            int8_t rowToRead = row + j;
            int8_t columnToRead = column + i;
            if (wrap) {
                     if (rowToRead    > 7) rowToRead    = 0;
                else if (rowToRead    < 0) rowToRead    = 7;
                     if (columnToRead > 7) columnToRead = 0;
                else if (columnToRead < 0) columnToRead = 7;
            }
            else {
                if (rowToRead    > 7 || rowToRead    < 0 ||
                    columnToRead > 7 || columnToRead < 0)
                        continue;
            }

            if ((j != 0 || i != 0) &&
                (rowToRead >= 0 && columnToRead >= 0) &&
                (rowToRead <= 7 && columnToRead <= 7) &&
                 bitRead(grid[rowToRead], columnToRead)) {
                    neighbors++;
            }
        }
    }
    return neighbors;
}

bool checkSerial() {
    static byte serialColumn = 0; // Current column that user is inputting
    static byte serialRow = 0; // Current row that user is inputting

    // If we're currently in the setting density mode
    static bool settingDensity = false; 

    while (Serial.available()) {
        byte incoming = Serial.read();

        if (settingDensity) settingDensity = changeDensity(incoming);

        switch (incoming) {

        case 'c':
            serialRow = serialColumn = 0;
            serialWriteEndOfBox();
            ignoreNewline();
            break;

        case 'D':
            settingDensity = changeDensity(incoming);

        case 's':
            generateRandomPattern(true);
            ignoreNewline();
            serialRow = serialColumn = 0;
            return true;
            break;
        
        case 'r':
            useRandom = true;
            ignoreNewline();
            break;

        case 'R':
            useRandom = false;
            ignoreNewline();
            break;
        
        case 'w':
            wrapEdges = true;
            ignoreNewline();
            break;

        case 'W':
            wrapEdges = false;
            ignoreNewline();
            break;

        case 'h':
            //Serial.println(); Serial.print(options);
            Serial.println(); Serial.print(F("Sorry, not implemented yet"));
            break;

        default:
            if (serialRow == 0 && serialColumn == 0) {
                serialWriteStartOfBox();
            }
            
            if (serialColumn == 0) {
                serialWriteStartOfRow(serialRow);
            }
            
            if ( incoming == '\n'){
                for (;serialColumn<8;serialColumn++) { 
                    // Set the rest of the row to dead
                    serialWriteDead(serialRow, serialColumn);
                }

                serialRow++;
                serialColumn = 0;
                serialWriteEndOfRow();
            }
            
            else if (serialColumn < 8) {

                if (incoming == '0' || incoming == ' ') {
                    serialWriteDead(serialRow, serialColumn);
                }
                else {
                    serialWriteAlive(serialRow, serialColumn);
                }
                serialColumn++;
            }
        }

        if (serialRow == 8) {
          serialRow = 0;
          serialWriteEndOfBox();
          return true;
        }
    }
    return false;
}

bool changeDensity(char incoming) {
    static byte densityBuf = 0;
    static byte i = 0;
    bool error = false;
    bool stillChanging = true;

    switch (i) {
    case 0: break;
    case 1: if (incoming != '='){ Serial.println("Not '='"); error = true;}
            break;
    case 2: case 3: case 4:
        if (incoming == '\n') {
            if (i==2) {error = true; Serial.println("short"); break; }
            // If there's one 1 digit, divide by 10
            if (i==3) densityBuf /= 10;
            density = densityBuf;
            densityBuf = 0;
            i = 0;
            Serial.print("density= "); Serial.println(density);
            stillChanging = false;
        }
        else if (incoming > '9'||incoming < '0') {Serial.println("not 0-9"); error = true;}
            
        else {
            densityBuf += (incoming - '0');
            // The first time will be the tens place
            if (i==2) densityBuf *= 10;
        }
        break;
    default: Serial.println("default"); error = true;
    }
    i++;

    if (error) {
        stillChanging = false;
        densityBuf = 0;
        i = 0;
        Serial.println("\nSyntax Error");
    }
    return stillChanging;
}

void ignoreNewline () {
    delayMicroseconds(200); // Wait for next character
    if (Serial.peek() == '\n') {
        Serial.read();
    }
}

void serialWriteDead(byte row, byte column) {
    Serial.write(' '); // Write a space
    // Clear the "column"th bit of serialgrid
    bitClear(serialBuffer[row], column);
}

void serialWriteAlive(byte row, byte column) {
    Serial.write(128); // Write a box character
    // Set the "column"th bit of serialgrid
    bitSet(serialBuffer[row], column);
}

void serialWriteStartOfBox() {
    Serial.println(F("\t --------"));
}

void serialWriteEndOfBox() {
    Serial.print(F("\t --------\nSetting...\n\n"));
}

void serialWriteStartOfRow(byte rowNum = 255) {
    if (rowNum != 255) Serial.print(rowNum);
    Serial.print("\t|");
}

void serialWriteEndOfRow() {
    Serial.println("|");
}

void generateRandomPattern(bool verbose) {
    if (verbose) serialWriteStartOfBox();
    for (byte j=0;j<8;j++) {
        if (verbose) serialWriteStartOfRow(j);
        for (byte i=0;i<8;i++) {
            if (random(100) > density) {
                if (verbose) serialWriteDead(j, i);
                else bitClear(randomBuffer[j], i);
            }
            else {
                if (verbose) serialWriteAlive(j, i);
                else bitSet(randomBuffer[j], i);
            }
        }
        if (verbose) serialWriteEndOfRow();
    }
    if (verbose) serialWriteEndOfBox();
}
