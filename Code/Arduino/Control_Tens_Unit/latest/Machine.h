#include <Arduino.h>

enum Status {ON, CHANNEL, MAGNITUDE_A, MAGNITUDE_B, MODE_A, MODE_B}; 
int DELAY = 7;

struct Machine
{
    /* data */
    int onPin;
    int offPin;
    int modePin;
    int swapPin;

    int deviceStatus[6] = {0,0,0,0,0,0};
    
    Machine(int on=9, int off=10, int mode=1, int swap=11)
    {
        onPin = on;
        offPin = off;
        modePin = mode;
        swapPin = swap;
    }

    int* showDeviceStatus() {
        return deviceStatus;
    }

    void SWITCH_ON() {
        if (!deviceStatus[ON]) {
            digitalWrite(onPin, HIGH);
            delay(DELAY);
            digitalWrite(onPin, LOW);
            delay(DELAY);
            deviceStatus[ON] = 1;
        }
        else return;
    }

    void swapChannel() {
        digitalWrite(swapPin, HIGH);
        delay(DELAY);
        digitalWrite(swapPin, LOW);
        delay(DELAY);
        deviceStatus[CHANNEL]++;
        deviceStatus[CHANNEL] %= 3;
    }

    void changeMode() {
        Serial.println("Changing mode");
        digitalWrite(modePin, HIGH);
        delay(DELAY);
        digitalWrite(modePin, LOW);
        delay(DELAY);
        if (deviceStatus[CHANNEL] == 0) {
            deviceStatus[MODE_A]++;
            deviceStatus[MODE_A] %= 8;
        }
        else if (deviceStatus[CHANNEL] == 1) {
            deviceStatus[MODE_B]++;
            deviceStatus[MODE_B] %= 8;
        }
        else if (deviceStatus[CHANNEL] == 2) {
            deviceStatus[MODE_A]++; deviceStatus[MODE_A] %= 8;
            deviceStatus[MODE_B]++; deviceStatus[MODE_B] %= 8;
        }
    }

    void increaseMagnitude() {
      Serial.println("Increasing magnitude");
      digitalWrite(onPin, HIGH);
      delay(DELAY);
      digitalWrite(onPin, LOW);
      delay(DELAY);
      if (deviceStatus[CHANNEL] == 0 && deviceStatus[MAGNITUDE_A] < 8) {
          deviceStatus[MAGNITUDE_A]++;
      }
      else if (deviceStatus[CHANNEL] == 1 && deviceStatus[MAGNITUDE_B] < 8) {
          deviceStatus[MAGNITUDE_B]++;
      }
      else if (deviceStatus[1] == 2) {
          if (deviceStatus[2] < 8) {
          deviceStatus[2]++;
          }
          if (deviceStatus[3] < 8) {
          deviceStatus[3]++;
          }
      }
    }

    // not complete
    void decreaseMagnitude() {
      Serial.println("Decreasing magnitude");
      digitalWrite(offPin, HIGH);
      delay(DELAY);
      digitalWrite(offPin, LOW);
      delay(DELAY);
      if (deviceStatus[CHANNEL] == 0 && deviceStatus[MAGNITUDE_A] > 0) {
          deviceStatus[MAGNITUDE_A]++;
      }
      else if (deviceStatus[CHANNEL] == 1 && deviceStatus[MAGNITUDE_B] > 0) {
          deviceStatus[MAGNITUDE_B]++;
      }
      else if (deviceStatus[CHANNEL] == 2) {
          if (deviceStatus[MAGNITUDE_A] < 8) {
          deviceStatus[MAGNITUDE_A]++;
          }
          if (deviceStatus[MAGNITUDE_B] < 8) {
          deviceStatus[MAGNITUDE_B]++;
          }
      }
    }

    // Now can be used
    void switchOff() {
        if (deviceStatus[0]) {
            digitalWrite(offPin, HIGH);
            delay(DELAY);
            digitalWrite(offPin, LOW);
            delay(DELAY);
        }
        else return;
    }
    
};
