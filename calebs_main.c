#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#include "sampler.h"
#include "LEDMatrix.h"
#include "joystick.h"


//#define USER_Button_Value "/sys/class/gpio/gpio72/value"

void handleExit() {
    
}

int isUserButtonPressed() {
   
    return 0; 
}

int main() {
    Sampler_startSampling();
     

    while (1) {
        if (isUserButtonPressed()) {
            handleExit();
            break;
        }
        sleep(1);  

        struct AnalysisData currentAnalysisData = Sampler_getAnalysisData();

        int dipsCount = currentAnalysisData.dips;
        double maxVoltage =  currentAnalysisData.maxVoltage;
        double minVoltage = currentAnalysisData.minVoltage;
        double minInterval = currentAnalysisData.minInterval;
        double maxInterval = currentAnalysisData.maxInterval;
        

        char bus[] = "/dev/i2c-1";

        int joystickDirection = Joystick_dir();

        switch (joystickDirection) {
        case 0:
            displayNext(bus, &dipsCount, INT);
            break;
        case 1:
            displayNext(bus, &maxVoltage, DOUBLE);
            break;
        case 2:
            displayNext(bus, &minVoltage, DOUBLE);
            break;
        case 3:
            displayNext(bus, &minInterval, DOUBLE);
            break;
        case 4:
            displayNext(bus, &maxInterval, DOUBLE);
            break;
        default:
            printf("Invalid direction\n");
            break;
        }

    }

    return 0;
}
