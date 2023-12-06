#include "sampler.h"
#include "LEDMatrix.h"
#include "joystick.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define userVal "/sys/class/gpio/gpio72/value"


int readFromFile(char* fileName) {
  FILE* pFile = fopen(fileName, "r"); 
  if(pFile == NULL) {
    printf("ERROR: Unable to open file (%s) for read\n", fileName);
    exit(-1);
  }
  const int MAX_LENGTH = 1024;
  char buff[MAX_LENGTH];
  fgets(buff, MAX_LENGTH, pFIle);
  fclose(pFile);
  int intValue = atoi(buff);
  return intValue;
}


void main(){
  int minVoltage = -1;
  int maxVoltage = -1;
  int avgVoltage = -1;

  int dips = -1;
  int samples = -1;

  int minTime = -1;
  int maxTime = -1;
  int avgTime = -1;

  int disp = initializeDisplay(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
  writeToDisplay(disp, 0x21, 0x00);
  writeToDisplay(disp, 0x81, 0x00);

  data analysisData = analysisFunction;
  
  while(readFromFile(userVal)==0){
      int prevMinVoltage = minVoltage;
      int prevMaxVoltage = maxVoltage;
      int prevAvgVoltage = avgVoltage;
    
      int prevDips = dips;
      int prevSamples = prevSamples;
    
      int prevMinTime = minTime;
      int prevMaxTime = maxTime;
      int prevAvgTime = avgTime;
    switch(Joystic_dir()){
      case 1:
        initializeFloatLED(disp, analysisData.maxVoltage);
        break;
      case 2:
        initializeFloatLED(disp, analysisData.minVoltage);
        break;
      case 3:
        initializeFloatLED(disp, analysisData.minInterval);
        break;
      case 4:
        initializeFloatLED(disp, analysisData.maxInterval);
        break;
      default:
        initializeLED(disp, analysisData.dips);
    }
  }
}
