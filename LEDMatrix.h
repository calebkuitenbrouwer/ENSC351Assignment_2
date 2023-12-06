// LEDMatrix.h

#ifndef _LEDMATRIXH
#define _LEDMATRIXH

// Enum to specify the type of data to display on the LED matrix
typedef enum {
    INT,    // Integer data type
    DOUBLE  // Double data type
} DataType;

// Function prototype for illuminating the LED matrix
void illuminateLED(char* I2C_FilePath, void* number, DataType type);

// Description: Writes a value to the specified register of the I2C device
static void writeI2cRegister(int i2cFileDesc, unsigned char regAddr, unsigned char value);

// Description: Displays the LED pattern on the 8x8 LED matrix
static void displayLED(int i2cFileDesc, unsigned char ledData[8]);

// Description: Initializes the I2C communication with the specified address
static int initializeI2c(char* bus, int address);

// Description: Initializes the LED pattern for an integer number
static void initializeLEDPatternInt(unsigned char ledData[8], int number);

// Description: Initializes the LED pattern for a floating-point number
static void initializeLEDPatternFloat(unsigned char ledData[8], float f);

#endif /* LED_MATRIX_H */
