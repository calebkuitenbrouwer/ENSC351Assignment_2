#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "LEDMatrix.h"

#define I2C_DEVICE_ADDRESS 0x27 

static unsigned char patterns[10][8] = {
    {0x07, 0x05, 0x05, 0x05, 0x05, 0x05, 0x07, 0x00}, // 0
    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00}, // 1
    {0x07, 0x04, 0x04, 0x07, 0x01, 0x01, 0x07, 0x00}, // 2
    {0x07, 0x04, 0x04, 0x07, 0x04, 0x04, 0x07, 0x00}, // 3
    {0x05, 0x05, 0x05, 0x07, 0x04, 0x04, 0x04, 0x00}, // 4
    {0x07, 0x01, 0x01, 0x07, 0x04, 0x04, 0x07, 0x00}, // 5
    {0x07, 0x01, 0x01, 0x07, 0x05, 0x05, 0x07, 0x00}, // 6
    {0x07, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x00}, // 7
    {0x07, 0x05, 0x05, 0x07, 0x05, 0x05, 0x07, 0x00}, // 8
    {0x07, 0x05, 0x05, 0x07, 0x04, 0x04, 0x07, 0x00}  // 9
};

static unsigned char patternsFloat[10][8] = {
    {0x07, 0x05, 0x05, 0x05, 0x05, 0x05, 0x07, 0x08}, // 0
    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x08}, // 1
    {0x07, 0x04, 0x04, 0x07, 0x01, 0x01, 0x07, 0x08}, // 2
    {0x07, 0x04, 0x04, 0x07, 0x04, 0x04, 0x07, 0x08}, // 3
    {0x05, 0x05, 0x05, 0x07, 0x04, 0x04, 0x04, 0x08}, // 4
    {0x07, 0x01, 0x01, 0x07, 0x04, 0x04, 0x07, 0x08}, // 5
    {0x07, 0x01, 0x01, 0x07, 0x05, 0x05, 0x07, 0x08}, // 6
    {0x07, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x08}, // 7
    {0x07, 0x05, 0x05, 0x07, 0x05, 0x05, 0x07, 0x08}, // 8
    {0x07, 0x05, 0x05, 0x07, 0x04, 0x04, 0x07, 0x08}  // 9
};

static void writeI2cRegister(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("Failed to write to I2C register");
        exit(-1);
    }
}

static void displayLED(int i2cFileDesc, unsigned char ledData[8]) {
    for (int i = 0; i < 16; i += 2) {
        writeI2cRegister(i2cFileDesc, i, ledData[i / 2]);
        writeI2cRegister(i2cFileDesc, i + 1, 0x00);
    }
}

static int initializeI2c(char* bus, int address) {
    int i2cFileDesc = open(bus, O_RDWR);
    if (i2cFileDesc < 0) {
        printf("I2C DRV: Failed to open bus for read/write (%s)\n", bus);
        perror("Error:");
        exit(-1);
    }

    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
        perror("Failed to set I2C device to slave address.");
        exit(-1);
    }
    return i2cFileDesc;
}

static void initializeLEDPatternInt(unsigned char ledData[8], int number) {
    if (number < 0 || number > 99) {
        fprintf(stderr, "Invalid input for integer LED pattern: %d\n", number);
        exit(-1);
    }
    int tens = number / 10;
    int ones = number % 10;
    for (int i = 0; i < 8; i++) {
        ledData[i] = (patterns[ones][i] << 4) | patterns[tens][i];
    }
}

static void initializeLEDPatternFloat(unsigned char ledData[8], float f) {
    if (f < 0.0 || f > 9.9) {
        fprintf(stderr, "Invalid input for float LED pattern: %.2f\n", f);
        exit(-1);
    }
    int number = (int)(f * 10);
    int tens = number / 10;
    int ones = number % 10;
    for (int i = 0; i < 8; i++) {
        ledData[i] = (patterns[ones][i] << 4) | patternsFloat[tens][i];
    }
}

void illuminateLED(char* I2C_FilePath, void* number, DataType type) {
    unsigned char ledData[8];
    
    int i2cFileDesc = initializeI2c(I2C_FilePath, I2C_DEVICE_ADDRESS);
    writeI2cRegister(i2cFileDesc, 0x21, 0x00);
    writeI2cRegister(i2cFileDesc, 0x81, 0x00);

    if (type == INT) {
        initializeLEDPatternInt(ledData, *((int*)number));
    } else if (type == DOUBLE) {
        initializeLEDPatternFloat(ledData, *((double*)number));
    }
    
    displayLED(i2cFileDesc, ledData);
    
    close(i2cFileDesc);
}

