const uint8_t serialRxPin  =  0;
const uint8_t serialTxPin  =  1;
const uint8_t encoder4BPin =  2;
const uint8_t encoder4APin =  3;
const uint8_t encoder3BPin =  4;
const uint8_t encoder3APin =  5;
const uint8_t mtr4pwmPin   =  6;
const uint8_t mtr4dirPin   =  7;
const uint8_t encoder2BPin =  8;
const uint8_t mtr3pwmPin   =  9;
const uint8_t mtr2pwmPin   = 10;
const uint8_t mtr1pwmPin   = 11;
const uint8_t encoder2APin = 12;
const uint8_t mtr3dirPin   = 13;
const uint8_t encoder1APin = 14;
const uint8_t encoder1BPin = 15;
const uint8_t mtr2dirPin   = 16;
const uint8_t mtr1dirPin   = 17;
const uint8_t i2cSDAPin    = 18;
const uint8_t i2cSCLPin    = 19;

const uint8_t mtrpwmPins[4] = {mtr1pwmPin, mtr2pwmPin, mtr3pwmPin, mtr4pwmPin};
const uint8_t mtrdirPins[4] = {mtr1dirPin, mtr2dirPin, mtr3dirPin, mtr4dirPin};
const uint8_t encoderAPins[4] = {encoder1APin, encoder2APin,
                                 encoder3APin, encoder4APin};
const uint8_t encoderBPins[4] = {encoder1BPin, encoder2BPin,
                                 encoder3BPin, encoder4BPin};
const uint8_t encoderPins[8] =  {encoder1APin, encoder1BPin,
                                 encoder2APin, encoder2BPin,
                                 encoder3APin, encoder3BPin,
                                 encoder4APin, encoder4BPin};

/* Notes:
 * If encoder pins change, change the code for reading pins in the interrupts
 */
